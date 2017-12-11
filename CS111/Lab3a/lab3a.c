// CS111 lab3a
// Jaehyung Park
// 504212821

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>

#define SUPERBLOCK_OFFSET 1024
#define min(a,b) ((a) < (b) ? (a) : (b))

// create a struct of super blocks (credit from: http://www.nongnu.org/ext2-doc/ext2.html)
// used the idea of implementing ext2.h header file (credit from: http://www.cs.toronto.edu/~bogdan/CSC369H-W2016/assignments/a3/ext2.h)
struct superblock {

	// superblock structure
	uint32_t s_inodes_count;
	uint32_t s_blocks_count;
	uint32_t s_r_blocks_count;
	uint32_t s_free_blocks_count;
	uint32_t s_free_inodes_count;
	uint32_t s_first_data_block;
	uint32_t s_log_block_size;
	int32_t s_log_frag_size;
	uint32_t s_blocks_per_group;
	uint32_t s_frags_per_group;
	uint32_t s_inodes_per_group;
	uint32_t s_mtime;
	uint32_t s_wtime;
	uint16_t s_mnt_count;
	uint16_t s_max_mnt_count;
	uint16_t s_magic;
	uint16_t s_state;
	uint16_t s_errors;
	uint16_t s_minor_rev_level;
	uint32_t s_lastcheck;
	uint32_t s_checkinterval;
	uint32_t s_creator_os;
	uint32_t s_rev_level;
	uint16_t s_def_resuid;
	uint16_t s_def_resgid;

	// EXT2_DYNAMIC_REV Specific
	uint32_t s_first_ino;
	uint16_t s_inode_size;
	uint16_t s_block_group_nr;
	uint32_t s_feature_compat;
	uint32_t s_feature_incompat;
	uint32_t s_feature_ro_compat;
	uint8_t s_uuid[16];
	uint8_t s_volume_name[16];
	uint8_t s_last_mounted[64];
	uint32_t s_algo_bitmap;

	// Performance Hints
	uint8_t s_prealloc_blocks;
	uint8_t s_prealloc_dir_blocks;
	uint16_t alignment;
	
	// Journaling Support
	uint8_t s_journal_uuid[16];
	uint32_t s_journal_inum;
	uint32_t s_journal_dev;
	uint32_t s_last_orphan;
	
	// Directory Indexing Support
	uint32_t s_hash_seed[4];
	uint8_t s_def_hash_version;
	uint8_t padding[3];
	
	// Other options
	uint32_t s_default_mount_options;
	uint32_t s_first_meta_bg;
	uint8_t unused[760];
} superblock;

struct blockgroup_descriptor {
	uint32_t bg_block_bitmap;
	uint32_t bg_inode_bitmap;
	uint32_t bg_inode_table;
	uint16_t bg_free_blocks_count;
	uint16_t bg_free_inodes_count;
	uint16_t bg_used_dirs_count;
	uint16_t bg_pad;
	uint8_t bg_reserved[12];
};

struct inodetable {
	uint16_t i_mode;
	uint16_t i_uid;
	uint32_t i_size;
	uint32_t i_atime;
	uint32_t i_ctime;
	uint32_t i_mtime;
	uint32_t i_dtime;
	uint16_t i_gid;
	uint16_t i_links_count;
	uint32_t i_blocks;
	uint32_t i_flags;
	uint32_t i_osd1;
	uint32_t i_block[15];
	uint32_t i_generation;
	uint32_t i_file_acl;
	uint32_t i_dir_acl;
	uint32_t i_faddr;
	uint8_t i_osd2[12];
};

struct linkeddirectory {
	uint32_t inode;
	uint16_t rec_len;
	uint8_t name_len;
	uint8_t file_type;
	int8_t name[255];
};

unsigned int block_size;
unsigned int block_groups;
unsigned int file_size;

// helper functions
// get/return n bits from bitamp
bool get_n_bit(const uint8_t* bitmap, unsigned n) {
	return (bitmap[n/8] >> n % 8) & 1;
}

// return block number
unsigned get_offset(unsigned num) {
	return num * block_size;
}

// check if block number is valid or not
bool is_valid_block(unsigned num) {
	return num >= superblock.s_first_data_block && num < superblock.s_blocks_count;
}

// get number of blocks for inode
unsigned block_nums(struct inodetable inode) {
	return inode.i_blocks * 512/block_size;
}

// get the offset of the block group descriptor table
unsigned block_group_dt_offset() {
	unsigned pos = SUPERBLOCK_OFFSET + sizeof(struct superblock);
	if (pos % block_size != 0) {
		pos = (pos / block_size + 1) * block_size;
	}
	return pos;
}

// allocate block number from indirect block to memory stackframe
uint32_t* get_indirect_block(int disk_image, uint32_t ib_offset) {
	uint32_t* indirect_block = malloc(block_size);
	pread(disk_image, indirect_block, block_size, ib_offset);
	for (unsigned i = 0; i < block_size/4; i++) {
		indirect_block[i] = get_offset(indirect_block[i]);
	}
	return indirect_block;
}

// load blocks of inode into memory offsets
uint32_t* block_offsets(int disk_image, const struct inodetable inode) {
	const unsigned block_num = block_nums(inode);
	uint32_t* blocks = malloc(block_num * 4);
	unsigned i = 0;
	
	// look for direct blocks
	for (unsigned direct_blocks = 0; direct_blocks < min(block_num, 12); direct_blocks++) {
		blocks[i++] = get_offset(inode.i_block[direct_blocks]);
	}
	if (i == block_num) {
		return blocks;
	}
	
	// look for indirect blocks
	uint32_t* indirect_block = get_indirect_block(disk_image, get_offset(inode.i_block[12]));
	memcpy(blocks + i, indirect_block, min(block_num - i, block_size/4) * 4);
	i += min(block_num - i, block_size/4);
	free(indirect_block);
	if (i == block_num) {
		return blocks;
	}
	
	// look for double indirect blocks
	uint32_t* double_indirect_block = get_indirect_block(disk_image, get_offset(inode.i_block[13]));
	for (unsigned indirect_block_num = 0; indirect_block_num < block_size/4; indirect_block_num++) {
		uint32_t* indirect_block = get_indirect_block(disk_image, double_indirect_block[indirect_block_num]);
		memcpy(blocks + i, indirect_block, min(block_num - i, block_size/4) * 4);
		i += min(block_num - i, block_size/4);
		free(indirect_block);
		if (i == block_num) {
			free(double_indirect_block);
			return blocks;
		}
	}
	free(double_indirect_block);
	
	// look for triple indirect blocks
	uint32_t* triple_indirect_block = get_indirect_block(disk_image, get_offset(inode.i_block[14]));
	for (unsigned double_indirect_block_num = 0; double_indirect_block_num < block_size/4; double_indirect_block_num++) {
		uint32_t* double_indirect_block = get_indirect_block(disk_image, get_offset(triple_indirect_block[double_indirect_block_num]));
		for (unsigned indirect_block_num = 0; indirect_block_num < block_size/4; indirect_block_num++) {
			uint32_t* indirect_block = get_indirect_block(disk_image, double_indirect_block[indirect_block_num]);
			memcpy(blocks + i, indirect_block, min(block_num - i, block_size/4) * 4);
			i += min(block_num - i, block_size/4);
			free(indirect_block);
			if (i == block_num) {
				free(double_indirect_block);
				free(triple_indirect_block);
				return blocks;
			}
		}
		free(double_indirect_block);
	}
	free(triple_indirect_block);
	
	return blocks;
}

// functions (each of the functions are implemented to execute and generate corresponding *.csv files)
// get superblock
void get_superblock(int disk_image, FILE* output_file) {
	pread(disk_image, &superblock, sizeof(struct superblock), SUPERBLOCK_OFFSET);
	block_size = 1024 << superblock.s_log_block_size;
	block_groups = superblock.s_blocks_count / superblock.s_blocks_per_group;
	fprintf(output_file, "%x,%u,%u,%u,%d,%u,%u,%u,%u\n", superblock.s_magic, superblock.s_inodes_count, superblock.s_blocks_count,
		block_size, superblock.s_log_frag_size >= 0 ? 1024 << superblock.s_log_frag_size : 1024 >> -superblock.s_log_frag_size,
		superblock.s_blocks_per_group, superblock.s_inodes_per_group, superblock.s_frags_per_group, superblock.s_first_data_block);
	
	// check for certain limitations
	if (superblock.s_magic != 0xEF53) {
		fprintf(stderr, "Error: invalid magic block: 0x%x\n", superblock.s_magic);
		exit(2);
	}
	if (block_size > 65536) {
		fprintf(stderr, "Error: invalid block size: %d\n", block_size);
		exit(2);
	}
	// perform sanity check
	struct stat stat;
	fstat(disk_image, &stat);
	file_size = stat.st_size;
	if (superblock.s_blocks_count > file_size/block_size) {
		fprintf(stderr, "Error: invalid block count %u is bigger than image size %u\n", superblock.s_blocks_count, file_size/block_size);
		exit(2);
	}
	if (superblock.s_first_data_block > file_size/block_size) {
		fprintf(stderr, "Error: invalid block count %u is bigger than image size %u\n", superblock.s_first_data_block, file_size/block_size);
		exit(2);
	}
	if (superblock.s_blocks_count % superblock.s_blocks_per_group != 0) {
		fprintf(stderr, "Error: %u blocks, %u blocks/group\n", superblock.s_blocks_count, superblock.s_blocks_per_group);
		exit(2);
	}
	if (superblock.s_inodes_count % superblock.s_inodes_per_group != 0) {
		fprintf(stderr, "Error: %u inodes, %u inodes/group\n", superblock.s_inodes_count, superblock.s_inodes_per_group);
		exit(2);
	}

}

// get group
void get_group(int disk_image, FILE* output_file) {

	// traver through the block group descriptor
	for (unsigned num = 0, pos = block_group_dt_offset(); num < block_groups; num++, pos += sizeof(struct blockgroup_descriptor)) {
		struct blockgroup_descriptor group_desc;
		pread(disk_image, &group_desc, sizeof(struct blockgroup_descriptor), pos);
		fprintf(output_file, "%u,%u,%u,%u,%x,%x,%x\n", superblock.s_blocks_per_group,
			group_desc.bg_free_blocks_count, group_desc.bg_free_inodes_count, group_desc.bg_used_dirs_count,
			group_desc.bg_inode_bitmap, group_desc.bg_block_bitmap, group_desc.bg_inode_table
		);
	}
}

// get bitmap
void get_bitmap(int disk_image, FILE* output_file) {
	unsigned block_num = 0;
	unsigned inode_num = 0;
	// traver through the block group descriptor
	for (unsigned num = 0, pos = block_group_dt_offset(); num < block_groups; num++, pos += sizeof(struct blockgroup_descriptor)) {
		struct blockgroup_descriptor group_desc;
		pread(disk_image, &group_desc, sizeof(struct blockgroup_descriptor), pos);
		
		// get free blocks
		uint8_t* mapping_blocks = malloc(superblock.s_blocks_per_group/8);
		pread(disk_image, mapping_blocks, superblock.s_blocks_per_group/8, get_offset(group_desc.bg_block_bitmap));
		for (; block_num < (num + 1) * superblock.s_blocks_per_group; block_num++) {
			if (get_n_bit(mapping_blocks, block_num % superblock.s_blocks_per_group) == 0) {
				fprintf(output_file, "%x,%u\n", group_desc.bg_block_bitmap, block_num + 1); 
			}
		}
		free(mapping_blocks);
		
		// get free inodes
		mapping_blocks = malloc(superblock.s_inodes_per_group/8);
		pread(disk_image, mapping_blocks, superblock.s_inodes_per_group/8, get_offset(group_desc.bg_inode_bitmap));
		for (; inode_num < (num + 1) * superblock.s_inodes_per_group; inode_num++) {
			int inode_num_local = inode_num % superblock.s_inodes_per_group;
			// inode number in this block
			if (get_n_bit(mapping_blocks, inode_num_local) == 0) {
				fprintf(output_file, "%x,%u\n", group_desc.bg_inode_bitmap, inode_num + 1);
			}
		}
		free(mapping_blocks);
	}
}

// helper function to get other 3 csv files
void allocate_input(int disk_image, FILE* output_file, void (*func)(int, FILE*, unsigned, struct inodetable)) 
{
	unsigned inode_num = 0;
	// traver through the block group descriptor
	for (unsigned num = 0, pos = block_group_dt_offset(); num < block_groups; num++, pos += sizeof(struct blockgroup_descriptor)) {
		struct blockgroup_descriptor group_desc;
		pread(disk_image, &group_desc, sizeof(struct blockgroup_descriptor), pos);

		uint8_t* mapping_inodes = malloc(superblock.s_inodes_per_group/8);
		pread(disk_image, mapping_inodes, superblock.s_inodes_per_group/8, get_offset(group_desc.bg_inode_bitmap));
		for (; inode_num < (num + 1) * superblock.s_inodes_per_group; inode_num++) {
			int inode_num_local = inode_num % superblock.s_inodes_per_group;
			if (get_n_bit(mapping_inodes, inode_num_local) == 1) {
				struct inodetable inode;
				pread(disk_image, &inode, sizeof(struct inodetable), get_offset(group_desc.bg_inode_table) + inode_num_local * sizeof(struct inodetable));
				func(disk_image, output_file, inode_num, inode);
			}
		}
		free(mapping_inodes);
	}
}

// get inode
void get_inode(int disk_image, FILE* output_file, unsigned inode_num, struct inodetable inode) {
	(void)disk_image;
	
	fprintf(output_file, "%d,%c,%o,%d,%d,%d,%x,%x,%x,%d,%d,",
	  inode_num + 1, inode.i_mode & 0x8000 ? 'f' : inode.i_mode & 0x4000 ? 'd' : inode.i_mode & 0xA000 ? 's' : '?',
	  inode.i_mode, inode.i_uid, inode.i_gid, inode.i_links_count, inode.i_ctime, inode.i_mtime, inode.i_atime,
	  inode.i_size, block_nums(inode));
	
	// print block pointers
	for (unsigned blck_ptr = 0; blck_ptr < 15; blck_ptr++) {
		fprintf(output_file, "%x", inode.i_block[blck_ptr]);
		fprintf(output_file, blck_ptr < 14 ? "," : "\n");
		
		if (inode.i_block[blck_ptr] != 0 && !is_valid_block(inode.i_block[blck_ptr]))
			fprintf(stderr, "inode %u has invalid block pointer[%u]: %d\n", inode_num + 1, blck_ptr, inode.i_block[blck_ptr]);
	}
}

// get directory
void get_directory(int disk_image, FILE* output_file, unsigned inode_num, struct inodetable inode) 
{		

	if (inode.i_mode & 0x4000) {
		unsigned directory_num = 0;
		uint32_t* blocks = block_offsets(disk_image, inode);
		
		for (unsigned blck_ptr = 0; blck_ptr < block_nums(inode); blck_ptr++) {
			unsigned directory_pos = blocks[blck_ptr];
			if (directory_pos == 0) {
				break;
			}
			
			do {
				struct linkeddirectory ld;
				pread(disk_image, &ld, sizeof(struct linkeddirectory), directory_pos);
				
				if (ld.inode != 0) {
					fprintf(output_file, "%u,%u,%u,%u,%u,%c%.*s%c\n", inode_num + 1,
					  directory_num, ld.rec_len, ld.name_len, ld.inode, '"', ld.name_len, ld.name, '"');
					
					if (ld.rec_len < 8 || ld.rec_len > 1024 || directory_pos + ld.rec_len > file_size) {
						fprintf(stderr, "inode %u, block %x in bad directory\n", inode_num + 1, blocks[blck_ptr]/block_size);
						return;
					}
					if (ld.name_len > ld.rec_len) {
						fprintf(stderr, "inode %u, block %x in bad directory: length = %u, namelength = %u\n", inode_num + 1, blocks[blck_ptr]/block_size, ld.rec_len, ld.name_len);
						return;
					}
					if (ld.inode > superblock.s_inodes_count) {
						fprintf(stderr, "inode %u, block %x in bad directory: inode = %u\n", inode_num + 1, blocks[blck_ptr]/block_size, ld.inode);
					}
				}
				directory_num++;
				directory_pos += ld.rec_len;
			} while (directory_pos < blocks[blck_ptr] + block_size);  
		}
		free(blocks);
	}
}

// get indirect
void get_indirect(int disk_image, FILE* output_file, unsigned inode_num, struct inodetable inode) {
	(void)inode_num;
	unsigned i = 12;

	const unsigned block_num = block_nums(inode);
	if (block_num <= 12) {
		return;
	}
	// look for indirect blocks
	uint32_t* indirect_block = get_indirect_block(disk_image, get_offset(inode.i_block[12]));
	unsigned limit = min(block_num - i, block_size/4);
	for (unsigned db_n = 0; db_n < limit; db_n++) {
		if (indirect_block[db_n] != 0) {
			fprintf(output_file, "%x,%u,%x\n", inode.i_block[12], db_n, indirect_block[db_n]/block_size);
			i++;

			if (!is_valid_block(indirect_block[db_n]/block_size)) {
				fprintf(stderr, "indirect block %x has an invalid entry[%u] = %x\n", inode.i_block[12], db_n, indirect_block[db_n]/block_size);
			}
		}
	}
	free(indirect_block);
	if (i == block_num) {
		return;
	}
	// look for double indirect blocks
	uint32_t* double_indirect_block = get_indirect_block(disk_image, get_offset(inode.i_block[13]));
	for (unsigned ib_n = 0; ib_n < block_size/4; ib_n++) {
		if (double_indirect_block[ib_n] != 0) {
			fprintf(output_file, "%x,%u,%x\n", inode.i_block[13], ib_n, double_indirect_block[ib_n]/block_size);
			

			if (!is_valid_block(double_indirect_block[ib_n]/block_size)) {
				fprintf(stderr, "indirect block %x has an invalid entry[%u] = %x\n", inode.i_block[13], ib_n, double_indirect_block[ib_n]/block_size);
				continue;
			}
		}
		uint32_t* indirect_block = get_indirect_block(disk_image, double_indirect_block[ib_n]);
		unsigned limit = min(block_num - i, block_size/4);
		for (unsigned db_n = 0; db_n < limit; db_n++) {
			if (indirect_block[db_n] != 0) {
				fprintf(output_file, "%x,%u,%x\n", double_indirect_block[ib_n]/block_size, db_n, indirect_block[db_n]/block_size);
				i++;

				if (!is_valid_block(indirect_block[db_n]/block_size)) {
					fprintf(stderr, "indirect block %x has an invalid entry[%u] = %x\n", double_indirect_block[ib_n]/block_size, db_n, indirect_block[db_n]/block_size);
				}
			}
		}
		free(indirect_block);
		if (i == block_num) {
			free(double_indirect_block);
			return;
		}
	}
	free(double_indirect_block);
	
	// look for triple indirect blocks
	uint32_t* triple_indirect_block = get_indirect_block(disk_image, get_offset(inode.i_block[14]));
	for (unsigned dib_n = 0; dib_n < block_size/4; dib_n++) {
		if (triple_indirect_block[dib_n] != 0) {
			fprintf(output_file, "%x,%u,%x\n", inode.i_block[14], dib_n, triple_indirect_block[dib_n]/block_size);

			if (!is_valid_block(triple_indirect_block[dib_n]/block_size)) {
				fprintf(stderr, "indirect block %x has an invalid entry[%u] = %x\n",inode.i_block[13], dib_n, triple_indirect_block[dib_n]/block_size);
				continue; 
			}
		}
		uint32_t* double_indirect_block = get_indirect_block(disk_image, get_offset(triple_indirect_block[dib_n]));
		for (unsigned ib_n = 0; ib_n < block_size/4; ib_n++) {
			if (double_indirect_block[ib_n] != 0) {
				fprintf(output_file, "%x,%u,%x\n", triple_indirect_block[dib_n]/block_size, ib_n, double_indirect_block[ib_n]/block_size);

				if (!is_valid_block(double_indirect_block[ib_n]/block_size)) {
					fprintf(stderr, "indirect block %x has an invalid entry[%u] = %x\n", triple_indirect_block[dib_n]/block_size, ib_n, 
					  double_indirect_block[ib_n]/block_size);
					continue; 
				}
			}
			uint32_t* indirect_block = get_indirect_block(disk_image, double_indirect_block[ib_n]);
			unsigned limit = min(block_num - i, block_size/4);
			for (unsigned db_n = 0; db_n < limit; db_n++) {
				if (indirect_block[db_n] != 0) {
					fprintf(output_file, "%x,%u,%x\n", double_indirect_block[ib_n]/block_size, db_n, indirect_block[db_n]/block_size);
					i++;

					if (!is_valid_block(indirect_block[db_n]/block_size)) {
						fprintf(stderr, "indirect block %x has an invalid entry[%u] = %x\n", double_indirect_block[ib_n]/block_size, db_n, indirect_block[db_n]/block_size);
					}
				}
			}
			free(indirect_block);
			if (i == block_num) {
				free(double_indirect_block);
				free(triple_indirect_block);
				return;
			}
		}
		free(double_indirect_block);
	}
	free(triple_indirect_block);
}

int main(int argc, char* argv[]) {
	
	// check if we passed the correct argument for the program
	if (argc != 2) {
		printf("file %s missing argument: disk-image\n", argv[0]);
		exit(1);
	}
	assert(sizeof(struct superblock) == 1024);
	assert(sizeof(struct blockgroup_descriptor) == 32);
	assert(sizeof(struct inodetable) == 128);
	assert(sizeof(struct linkeddirectory) == 264);
	
	// open the disk-image file
	int read_from_disk = open(argv[1], O_RDONLY);

	// execute the program outputting related *.csv files
	// we need to output its summary files (total 6 different files)
	FILE* fd1 = fopen("super.csv", "w");
	get_superblock(read_from_disk, fd1);
	fclose(fd1);
	
	FILE* fd2 = fopen("group.csv", "w");
	get_group(read_from_disk, fd2);
	fclose(fd2);
	
	FILE* fd3 = fopen("bitmap.csv", "w");
	get_bitmap(read_from_disk, fd3);
	fclose(fd3);
	
	FILE* fd4 = fopen("inode.csv", "w");
	allocate_input(read_from_disk, fd4, get_inode);
	fclose(fd4);
	
	FILE* fd5 = fopen("directory.csv", "w");
	allocate_input(read_from_disk, fd5, get_directory);
	fclose(fd5);
	
	FILE* fd6 = fopen("indirect.csv", "w");
	allocate_input(read_from_disk, fd6, get_indirect);
	fclose(fd6);
	
	// Finish
	close(read_from_disk);
}
