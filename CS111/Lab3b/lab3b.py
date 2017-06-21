# CS 111 lab3b
# Jaehyung Park
# 504-212-821

## From the lab description, we need to output the correct format that is given
# for instance for unallocated blocks, the INODE should be listed in increasing order of the inode_num
# as it follows the format: UNALLOCATED BLOCK < block_num > REFERENCED BY (INODE < inode_num > (INDIRECT BLOCK < block_num>) ENTRY < entry_num >) * n

import csv # import csv to deal with csv files
## the main data structure will be using dictionary

# create empty list
superblock = []
group = []
bitmap = []
inode = []
directory = []
indirect = []

# create empty dictionary to store directly and indirectly allocated block ids
direct_id = {}
indirect_id = {}

# create empty dictionary to allocated inodes for inode and entry number
locate_inodes = {}

# below are the methods
def get_direct_block(block_num):
	if block_num not in direct_id:
		return None
	return direct_id[block_num]

def get_indirect_block(block_num):
	if block_num not in indirect_id:
		return None
	output = []
	for (indirect_num, entry_num) in indirect_id[block_num]:
		if indirect_num in direct_id:
			indirect_inode = [inode_num2 for (inode_num2, entry_num2) in get_direct_block(indirect_num)]
		elif indirect_num in indirect_id:
			indirect_inode = [inode_num2 for (inode_nums2, indirect_num2, entry_num2)
				in get_indirect_block(indirect_num) for inode_num2 in inode_nums2]
		output.append((indirect_inode, indirect_num, entry_num))
	return output

## for unallocated_block, duplicate_block, and invalid_block_pointer, the output
# shares the same output form of INODE < inode_num > (INDIRECT BLOCK < block_num>) ENTRY < entry_num >
# which traces the Inode number and block number
def get_inode_num(block_num):
	output = []
	if block_num in direct_id:
		for (inode_num, entry_num) in get_direct_block(block_num):
			output.append((inode_num, "INODE < " + str(inode_num) + " > ENTRY < " + str(entry_num) + " >"))
	if block_num in indirect_id:
		for (inode_nums, indirect_num, entry_num) in get_indirect_block(block_num):
			for inode_num in inode_nums:
				output.append((inode_num, "INODE < " + str(inode_num) + " > INDIRECT BLOCK < " + str(indirect_num) + " > ENTRY < " + str(entry_num) + " >"))
	return [outputs[1] for outputs in sorted(output)]

def get_string_lists(inode_num):
	output = []
	if inode_num in locate_inodes:
		for (inode_dir, entry_num) in locate_inodes[inode_num]:
			output.append((inode_num, "DIRECTORY < " + str(inode_dir) + " >  ENTRY < " + str(entry_num) + " >"))
	return [outputs[1] for outputs in sorted(output)]

# check for unallocated block - output
# UNALLOCATED BLOCK < block_num > REFERENCED BY 
# (INODE < inode_num > (INDIRECT BLOCK < block_num>) ENTRY < entry_num >) * n
def check_unallocated_block():
	bitmap_nums = set([int(row[5], 16) for row in group])
	block_ids = [int(row[1]) for row in bitmap if int(row[0], 16) in bitmap_nums]

	for block_num in block_ids:
		output = get_inode_num(block_num)
		if output:
			print "UNALLOCATED BLOCK < " + str(block_num) + " > REFERENCED BY",
			for outputs in output:
				print outputs,
			print

# check for duplicate block - output
# MULTIPLY REFERENCED BLOCK < block_num > BY 
# (INODE < inode_num > (INDIRECT BLOCK < block_num>) ENTRY < entry_num >) * n
def check_duplicate_block():
	block_ids = direct_id.keys() + indirect_id.keys()	# get keys in the dictionary
	for block_num in block_ids:
		check_block = int(superblock[0][8])
		count_block = int(superblock[0][2])
		if block_num >= check_block and block_num < count_block:
			output = get_inode_num(block_num)
			if len(output) > 1:
				print "MULTIPLY REFERENCED BLOCK < " + str(block_num) + " > BY",
				for outputs in output:
					print outputs,
				print

# check for unallocated inode - output
# UNALLOCATED INODE < inode_num > REFERENCED BY 
# (DIRECTORY < inode_num > ENTRY < entry_num >) * n
def check_unallocated_inode():
	bitmap_ids = set([int(row[4], 16) for row in group])
	free_inode = [int(row[1]) for row in bitmap if int(row[0], 16) in bitmap_ids]
	for inode_num in free_inode:
		output = get_string_lists(inode_num)
		if output:
			print "UNALLOCATED INODE < " + str(inode_num) + " > REFERENCED BY",
			for outputs in output:
				print outputs,
			print

# check for missing inode - output
# MISSING INODE < inode_num > SHOULD BE IN FREE LIST < block_num >
def check_missing_inode():
	bitmap_ids = [int(row[4], 16) for row in group]
	free_inode = set([int(row[1]) for row in bitmap if int(row[0], 16) in bitmap_ids])
	group_inodes = int(superblock[0][6])
	inode_count = int(superblock[0][1])

	for inode_num in range(11, inode_count):
		if inode_num not in free_inode and inode_num not in locate_inodes:
			block_num = bitmap_ids[inode_num // group_inodes]
			print "MISSING INODE < " + str(inode_num) + " > SHOULD BE IN FREE LIST < " + str(block_num) + " >"

# check for incorrect link count - output
# LINKCOUNT < inode_num > IS < link_count > SHOULD BE < link_count >
def check_incorrect_link_count():
	# iterate through the rows of inode
	for row in inode:
		inode_num = int(row[0])	# first index of row in inode
		count_link = int(row[5])
		real_count_link = len(get_string_lists(inode_num))
		if count_link != real_count_link:
			print "LINKCOUNT < " + str(inode_num) + " > IS < " + str(count_link) + " > SHOULD BE < " + str(real_count_link) + " >"

# check for incorrect directory entry - output
# INCORRECT ENTRY IN < inode_num > NAME < entry_name > 
# LINK TO < inode_num > SHOULD BE < inode_num >
def check_incorrect_directory_entry():
	# iterate through the rows of directory
	for row in directory:
		inode_num = int(row[0])	# first index of row in directory
		entry_num = int(row[4])	# fifth index of row in directory
		name = row[5]	# sixth index of row in directory
		# look at '.' and '..' entries that don't link to correct inodes
		if name == "." and inode_num != entry_num:
			print "INCORRECT ENTRY IN < " + str(inode_num) + " > NAME < " + name + " > LINK TO < " + str(entry_num) + " > SHOULD BE < " + str(inode_num) + " >"
		parent_inode = locate_inodes[inode_num][0][0]
		if name == ".." and parent_inode != entry_num:
			print "INCORRECT ENTRY IN < " + str(inode_num) + " > NAME < " + name + " > LINK TO < " + str(entry_num) + " > SHOULD BE < " + str(parent_inode) + " >"

# check for invalid block pointer - output
# INVALID BLOCK < block_num > IN INODE < inode_num > (INDIRECT BLOCK < block_num >) ENTRY < entry_num >
def check_invalid_block_pointer():
	block_ids = direct_id.keys() + indirect_id.keys()	# get keys in the dictionary
	for block_num in block_ids:
		check_block = int(superblock[0][8])	# check the block
		count_block = int(superblock[0][2])	# check the count of block
		if not block_num >= check_block or not block_num < count_block:
			output = get_inode_num(block_num)
			for outputs in output:
				print "INVALID BLOCK < " + str(block_num) + " > IN " + outputs

def get_list(in_file):
	with open(in_file, "r") as csvfile:
		return [row for row in csv.reader(csvfile)]

## main
if __name__ == "__main__":

	superblock = get_list("super.csv")
	group = get_list("group.csv")
	bitmap = get_list("bitmap.csv")
	inode = get_list("inode.csv")
	directory = get_list("directory.csv")
	indirect = get_list("indirect.csv")

	# declare variables
	block_size = int(superblock[0][3])
	direct_limit = 12
	indirect_limit = direct_limit + block_size/4
	double_indirect_limit = indirect_limit + (block_size/4)**2
	triple_indirect_limit = double_indirect_limit + (block_size/4)**3

	for row in inode:
		block_nums = int(row[10])
		total_limit = block_nums if block_nums <= direct_limit else 13 if block_nums <= indirect_limit else 14 if block_nums <= double_indirect_limit else 15  
		for i in range(total_limit):
			block_num = int(row[11 + i], 16)
			if block_num in direct_id:
				direct_id[block_num].append((int(row[0]), i))
			else:
				direct_id[block_num] = [(int(row[0]), i)]

	for row in indirect:
		block_num = int(row[2], 16)
		if block_num in indirect_id:
			indirect_id[block_num].append((int(row[0], 16), int(row[0], 16)))
		else:
			indirect_id[block_num] = [(int(row[0], 16), int(row[1], 16))]

	for row in directory:
		inode_num = int(row[4])
		if inode_num in locate_inodes:
			locate_inodes[inode_num].append((int(row[0]), int(row[1])))
		else:
			locate_inodes[inode_num] = [(int(row[0]), int(row[1]))]

	check_unallocated_block()
	check_duplicate_block()
	check_unallocated_inode()
	check_missing_inode()
	check_incorrect_link_count()
	check_incorrect_directory_entry()
	check_invalid_block_pointer()