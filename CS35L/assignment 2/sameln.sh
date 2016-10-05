#!/bin/sh
dir=$1

declare -a ARRAY
let count=0
# the given spec says we should consider files starting with . and
# put them in priority
RESULT=`ls -a $dir | egrep '^\.' | sort`
for FILE in $RESULT
do
  if [ ! -L "$DIR/$FILE" ]
  then
    if [ ! -f "$DIR/$FILE" ]
    then
      echo "$dir/$FILE is a regular file."
      ARRAY[$count]="$dir/$FILE"
      let count=count+1
    elif [ ! -r "$dir/$FILE" ]
    then
      ehco "$dir/$FILE is NOT a regular file."
    fi
  fi
done

# create an array of regular files and store the files in the same array
RESULT=`ls -a $dir | sort`
for FILE in $RESULT
do
   if [ ! -L "$DIR/$FILE" ]
   then
    if [ ! -f "$dir/$FILE" ]
    then
      echo "$dir/$FILE is a regular file."
      ARRAY[$count]="$dir/$FILE"
      let count=count+1
    elif [ ! -r "$dir/$FILE" ]
    then
      echo "$dir/$FILE is NOT a regular file."
    fi
   fi
done

# since lexicographically, . dot files come first and non dot files comes
# later, so adding dot files first and adding non dot files in the array 
# already contains sorted files.

# compare and loop to make hardlink of duplicates
let newcount=0 # let initial pointer to 0
while [ $newcount -lt $count ]
do
  let tempcount=$newcount+1
  while [ $tempcount -lt $count ]
  do
    # compare
    cmp -s "${ARRAY[$newcount]}" "$ARRAY[$tempcount]}"
    if [ $? == 0 ] # if both files are the same
    then # remove that duplicate and make hardlink of the first file
      rm "${ARRAY[$tempcount]}"
      ln "${ARRAY[$newcount]}" "$ARRAY[$tempcount]}"
    fi
    # increment the pointer to traverse the array
    let tempcount=tempcount+1
  done
  let newcount=newcount+1
done
