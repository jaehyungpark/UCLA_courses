# Project 3B
# lab3b.py
#
# Kevin Hsieh
# 27 November 2016
# CS 111 (1C)

import csv

# ------------------------------------------------------------------------------
# DATA STRUCTURES FOR HOLDING CSV DATA; UTILITY FUNCTIONS
# ------------------------------------------------------------------------------

super = []
group = []
bitmap = []
inode = []
directory = []
indirect = []

def load_to_list(csv_file_name):
	with open(csv_file_name, "r") as csv_file:
		return [row for row in csv.reader(csv_file)]

# Returns whether or not a given block id is valid.
def is_valid_block_id(block_id):
	s_first_data_block = int(super[0][8])
	s_blocks_count = int(super[0][2])
	return block_id >= s_first_data_block and block_id < s_blocks_count

# ------------------------------------------------------------------------------
# (BLOCK->) INODE TRACING
# ------------------------------------------------------------------------------

# dictionary of directly allocated block (dab) ids 
#   -> [(inode_n, entry_n)]	
dab_ids = {}

# dictionary of indirectly allocated block (iab) ids 
#   -> [(parent indirect block id, entry_n)]
iab_ids = {}

# Initializes data structures for inode tracing.
def inode_trace_init():
	# do some calculations regarding indirect blocks
	ss_block_size = int(super[0][3])
	db_limit = 12
	ib_limit = db_limit + ss_block_size/4
	dib_limit = ib_limit + (ss_block_size/4) ** 2
	tib_limit = dib_limit + (ss_block_size/4) ** 3
	
	for row in inode:
		# find number of valid block pointers within the inode itself
		i_blocks = int(row[10])
		limit = i_blocks if i_blocks <= db_limit else\
		  13 if i_blocks <= ib_limit else\
		  14 if i_blocks <= dib_limit else\
		  15  # presumably, i_blocks < tib_limit
		
		for i in range(limit):
			block_id = int(row[11 + i], 16)
			if block_id in dab_ids:
				dab_ids[block_id].append((int(row[0]), i))
			else:
				dab_ids[block_id] = [(int(row[0]), i)]
	for row in indirect:
		block_id = int(row[2], 16)
		if block_id in iab_ids:
			iab_ids[block_id].append((int(row[2], 16), int(row[0], 16)))
		else:
			iab_ids[block_id] = [(int(row[0], 16), int(row[1], 16))]

# Returns [(inode_n, entry_n)] for a directly allocated block
def trace_dab(block_id):
	if block_id not in dab_ids:
		return None
	return dab_ids[block_id]

# Returns [([inode_ns], ib_id, entry_n)] for an indirectly allocated block
def trace_iab(block_id):
	if block_id not in iab_ids:
		return None
	ret = []
	for (ib_id, entry_n) in iab_ids[block_id]:
		if ib_id in dab_ids:
			inode_ns = [inode_n2 for (inode_n2, entry_n2) in trace_dab(ib_id)]
		elif ib_id in iab_ids:
			inode_ns = [inode_n2 for (inode_ns2, ib_id2, entry_n2) 
			  in trace_iab(ib_id) for inode_n2 in inode_ns2]
		ret.append((inode_ns, ib_id, entry_n))
	return ret

# Returns a list of strings, each of which describes a reference to the block id
def inode_trace_references(block_id):
	references = []
	if block_id in dab_ids:
		for (inode_n, entry_n) in trace_dab(block_id):
			references.append((inode_n, 
			 "INODE < " + str(inode_n) + " > " +\
			 "ENTRY < " + str(entry_n) + " >"))
	if block_id in iab_ids:
		for (inode_ns, ib_id, entry_n) in trace_iab(block_id):
			for inode_n in inode_ns:
				references.append((inode_n,
				  "INODE < " + str(inode_n) + " > " +\
				  "INDIRECT BLOCK < " + str(ib_id) + " > " +\
				  "ENTRY < " + str(entry_n) + " >"))
	return [reference[1] for reference in sorted(references)]

# ------------------------------------------------------------------------------
# (INODE->) DIRECTORY TRACING
# ------------------------------------------------------------------------------
	
# dictionary of allocated inodes
#   -> [(dir_inode_n, entry_n)]
allocated_inodes = {}

# Initializes data structures for directory tracing.
def dir_trace_init():
	for row in directory:
		inode_n = int(row[4])
		if inode_n in allocated_inodes:
			allocated_inodes[inode_n].append((int(row[0]), int(row[1])))
		else:
			allocated_inodes[inode_n] = [(int(row[0]), int(row[1]))]

# Returns a list of strings, each of which describes a reference to the inode
def dir_trace_references(inode_n):
	references = []
	if inode_n in allocated_inodes:
		for (dir_inode_n, entry_n) in allocated_inodes[inode_n]:
			references.append((inode_n, 
			 "DIRECTORY < " + str(dir_inode_n) + " > " +\
			 "ENTRY < " + str(entry_n) + " >"))
	return [reference[1] for reference in sorted(references)]
	
# ------------------------------------------------------------------------------
# PROGRAM FUNCTIONS
# ------------------------------------------------------------------------------

# Check unallocated blocks.
def check_1():
	# set of block ids where there is a block bitmap
	block_bitmap_ids = set([int(row[5], 16) for row in group])
	
	# list of free block ids (exclude entries from inode bitmaps)
	free_block_ids = [int(row[1]) for row in bitmap if int(row[0], 16) 
	  in block_bitmap_ids]
		
	# iterate through supposedly free blocks
	for block_id in free_block_ids:
		references = inode_trace_references(block_id)
		if references:
			print "UNALLOCATED BLOCK < " + str(block_id) + " > REFERENCED BY",
			for reference in references:
				print reference,
			print

# Check for duplicately allocated blocks.
def check_2():
	allocated_block_ids = dab_ids.keys() + iab_ids.keys()
	
	# iterate through allocated block ids
	for block_id in allocated_block_ids:
		if is_valid_block_id(block_id):
			references = inode_trace_references(block_id)
			if len(references) > 1:
				print "MULTIPLY REFERENCED BLOCK < " + str(block_id) + " > BY",
				for reference in references:
					print reference,
				print

# Check for unallocated inodes.
def check_3():
	# set of block ids where there is an inode bitmap
	inode_bitmap_ids = set([int(row[4], 16) for row in group])
	
	# list of free inodes (exclude entries from block bitmaps)
	free_inode_ns = [int(row[1]) for row in bitmap if int(row[0], 16) 
	  in inode_bitmap_ids]
	
	# iterate through supposedly free inodes
	for inode_n in free_inode_ns:
		references = dir_trace_references(inode_n)
		if references:
			print "UNALLOCATED INODE < " + str(inode_n) + " > REFERENCED BY",
			for reference in references:
				print reference,
			print

# Check for missing inodes.
def check_4():
	# list of block ids where there is an inode bitmap
	inode_bitmap_ids = [int(row[4], 16) for row in group]
	
	# set of free inodes (exclude entries from block bitmaps)
	free_inode_ns = set([int(row[1]) for row in bitmap if int(row[0], 16) 
	  in inode_bitmap_ids])
	
	# get some superblock values
	s_inodes_count = int(super[0][1])
	s_inodes_per_group = int(super[0][6])
	
	# iterate through all inode numbers
	for inode_n in range(11, s_inodes_count):  # ignore first 10 inodes
		if inode_n not in free_inode_ns and inode_n not in allocated_inodes:
			print "MISSING INODE < " + str(inode_n) +\
			  " > SHOULD BE IN FREE LIST < " +\
			  str(inode_bitmap_ids[inode_n // s_inodes_per_group]) + " >"

# Check for incorrect link counts.
def check_5():
	# iterate through allocated inodes
	for row in inode:
		inode_n = int(row[0])
		link_count = int(row[5])
		actual_link_count = len(dir_trace_references(inode_n))
		if link_count != actual_link_count:
			print "LINKCOUNT < " + str(inode_n) + " > IS < " +\
			  str(link_count) + " > SHOULD BE < " + str(actual_link_count) +\
			  " >"

# Check for incorrect directory entries.
def check_6():
	for row in directory:
		dir_inode_n = int(row[0])
		entry_inode_n = int(row[4])
		name = row[5]
		if name == "." and dir_inode_n != entry_inode_n:
			print "INCORRECT ENTRY IN < " + str(dir_inode_n) + " > NAME < " +\
			  name + " > LINK TO < " + str(entry_inode_n) + " > SHOULD BE < " +\
			  str(dir_inode_n) + " >"
		parent_inode_n = allocated_inodes[dir_inode_n][0][0]
		if name == ".." and parent_inode_n != entry_inode_n:
			print "INCORRECT ENTRY IN < " + str(dir_inode_n) + " > NAME < " +\
			  name + " > LINK TO < " + str(entry_inode_n) + " > SHOULD BE < " +\
			  str(parent_inode_n) + " >"

# Check for invalid block pointers.
def check_7():
	allocated_block_ids = dab_ids.keys() + iab_ids.keys()
	
	# iterate through allocated block ids
	for block_id in allocated_block_ids:
		if not is_valid_block_id(block_id):
			references = inode_trace_references(block_id)
			for reference in references:
				print "INVALID BLOCK < " + str(block_id) + " > IN " + reference

# ------------------------------------------------------------------------------
# MAIN
# ------------------------------------------------------------------------------
			
if __name__ == "__main__":
	super = load_to_list("super.csv")
	group = load_to_list("group.csv")
	bitmap = load_to_list("bitmap.csv")
	inode = load_to_list("inode.csv")
	directory = load_to_list("directory.csv")
	indirect = load_to_list("indirect.csv")
	
	inode_trace_init()
	dir_trace_init()
	check_1()
	check_2()
	check_3()
	check_4()
	check_5()
	check_6()
	check_7()
