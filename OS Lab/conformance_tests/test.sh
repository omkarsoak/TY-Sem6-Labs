#!/bin/bash

# Function to check if a command was successful
check_success() {
    if [ $? -eq 0 ]; then
        echo "pass"
    else
        echo "fail"
    fi
}

gcc read100.c -o read100
gcc read510.c -o read510
gcc write10end.c -o write10end
gcc copytest.c -o copytest
gcc renametest.c -o renametest

# Test read100
./read100  # Run the C program to write data to the file

echo -n "read100: "
cat /tmp/test_file.txt | head -c 100 | diff - <(yes A | head -c 100)
check_success

# Test read510
read510_output=$(./read510)  # Call the C program to read bytes 5 to 10 from the file

echo -n "read510: "
expected_output="omkaro"
#echo -e "Actual Output: $read510_output\nExpected Output: $expected_output"
if [ "$read510_output" = "$expected_output" ]; then
    echo "pass"
else
    echo "fail"
fi


# Run the C program to write 10 bytes at the end of the file
read_buffer=$(./write10end | sed 's/[\n:]//g')
# Test write10end
echo -n "write10end: "
expected_output="123456789"
#echo -e "Actual Output: $read_buffer\n Expected Output: $expected_output"
if [ "$read_buffer" = "$expected_output" ]; then
    echo "pass"
else
    echo "fail"
fi

# Test copy
./copytest
echo -n "copy: "
diff /tmp/test_file4.txt /tmp/copied_file4.txt > /dev/null
check_success



# Test rename
./renametest
echo -n "rename: "

# Check if the old file still exists
if [ -e "/tmp/test_file5.txt" ]; then
    echo "fail: Old file still exists"
    exit 1
fi

# Check if the renamed file exists
if [ ! -e "/tmp/renamed_file5.txt" ]; then
    echo "fail: Renamed file does not exist"
    exit 1
fi
check_success


