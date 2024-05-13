/*OMKAR OAK OS Lab Assignment - inode*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include <ext2fs/ext2_fs.h>

#define BLOCK_SIZE 1024

//get the inode information from the disk
struct ext2_inode getInode(int fd, int ino) 
{
    //hold file system and inode information
    struct ext2_super_block sb;
    struct ext2_group_desc bgdesc;
    struct ext2_inode inode;
    long inode_offset;

    //Read super block
    lseek64(fd, 1024, SEEK_SET);
    read(fd, &sb, sizeof(struct ext2_super_block));

    //calculate block size
    int block_size = BLOCK_SIZE << sb.s_log_block_size;
    int bgno = (ino - 1) / sb.s_inodes_per_group;
    int iindex = (ino - 1) % sb.s_inodes_per_group;

    //read group descriptor and locate inode table
    lseek64(fd, block_size + bgno * sizeof(bgdesc), SEEK_SET);
    read(fd, &bgdesc, sizeof(struct ext2_group_desc));

    //calculate offset of the inode
    inode_offset = bgdesc.bg_inode_table * block_size + iindex * sb.s_inode_size;
    lseek64(fd, inode_offset, SEEK_SET);
    read(fd, &inode, sizeof(inode));

    return inode;
}

//read block from the disk
struct ext2_dir_entry_2* readBlock(int fd, int block_number) 
{
    struct ext2_dir_entry_2* dir_array = malloc(BLOCK_SIZE);

    lseek64(fd, block_number * BLOCK_SIZE, SEEK_SET);
    read(fd, dir_array, BLOCK_SIZE);  //read block

    return dir_array;
}

//returns inode number from path
int resolvePath(int fd, char* path, struct ext2_inode inode, int block_size) 
{
    char* component = strtok(path, "/");  //tokenize the path
    while (component != NULL) 
    {
        //read directory block
        struct ext2_dir_entry_2 *dir_array = readBlock(fd, inode.i_block[0]);
        int size = inode.i_size;
        int offset = 0;

        //go over all dir entries
        while (offset < size) 
        {
            struct ext2_dir_entry_2* entry = (struct ext2_dir_entry_2 *)((char*)dir_array + offset);
            char name[EXT2_NAME_LEN + 1];
            strncpy(name, entry->name, entry->name_len);
            name[entry->name_len] = '\0';

            //compare component with dir entry name
            if (strcmp(component, name) == 0) 
            {
                if ((entry->file_type & EXT2_FT_DIR) == EXT2_FT_DIR) 
                {
                    inode = getInode(fd, entry->inode);  //if its a dir, update inode and continue
                    break;
                } 
                else 
                {
                    return entry->inode;  //if its a file, return inode
                }
            }
            offset += entry->rec_len;
        }
        component = strtok(NULL, "/");
    }
    return -1;
}

//prints inode information
void printInfo(int fd, int inode_number) 
{
    struct ext2_inode inode = getInode(fd, inode_number);

    //print inode details
    printf("Size of file: %d\n", inode.i_size);
    printf("Mode: %04o\n", inode.i_mode & 0777);
    printf("Number of blocks: %d\n", inode.i_blocks);

    //print block numbers
    for (int i = 0; i < inode.i_blocks; i++) 
    {
        if (inode.i_block[i] == 0) 
        {
            break;
        }
        printf("Block %d: %d\n", i + 1, inode.i_block[i]);
    }
}

//print data
void printData(int fd, int inode_number, int block_size) 
{
    struct ext2_inode inode = getInode(fd, inode_number);
    
    //check if inode is a dir
    if (!(inode.i_mode & EXT2_S_IFDIR)) 
    {
        printf("Error: Not a directory\n");
        return;
    }
    
    //read directory block
    struct ext2_dir_entry_2 *dir_array = readBlock(fd, inode.i_block[0]);
    int size = inode.i_size;
    int offset = 0;
    
    while (offset < size)   //go over directory entries and print names 
    {
        struct ext2_dir_entry_2* entry = (struct ext2_dir_entry_2 *)((char*)dir_array + offset);
        
        char name[EXT2_NAME_LEN + 1];
        strncpy(name, entry->name, entry->name_len);
        name[entry->name_len] = '\0';
        printf("%s\n", name);
        
        offset += entry->rec_len;
    }
}

int main(int argc, char* argv[]) 
{ 
    if (argc != 4) 
    {
        exit(EXIT_FAILURE);
    }
    
    int fd = open(argv[1], O_RDONLY);  //open disk image
    if (fd == -1) 
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char* path = argv[2];
    char* mode = argv[3];

    int root_inode_number = 2;
    struct ext2_inode root_inode = getInode(fd, root_inode_number);

    int inode_number = resolvePath(fd, path, root_inode, BLOCK_SIZE);    //resolve path to get inode
    if (inode_number == -1) 
    {
        printf("Error: Unable to resolve path\n");
        exit(EXIT_FAILURE);
    }


    //main if-else: based on mode
    if (strcmp(mode, "inode") == 0) 
    {
        printInfo(fd, inode_number);
    } 
    else if (strcmp(mode, "data") == 0) 
    {
        printData(fd, inode_number, BLOCK_SIZE);
    } 
    else 
    {
        printf("Invalid mode: Use 'inode' or 'data'\n");
        exit(EXIT_FAILURE);
    }

    close(fd);
    
    return 0;
}

