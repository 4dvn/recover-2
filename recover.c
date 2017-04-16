/**
 * Recovers JPEGs from a forensic image.
 */
       
#include <stdio.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }
    
    // open file
    FILE *file = fopen(argv[1], "r");
    
    // if file can't be opened for reading, inform the user
    if (file == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }
    
    // create a buffer
    uint8_t buffer[512];
    
    // for naming and writing new files
    char filename[8];
    int counter = 0;
    
    // file pointer for jpegs
    FILE *img = NULL;
    
    // repeat until end of card
    while (fread(buffer, 512, 1, file)) 
    {
        // find jpeg
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0) 
        {
            
            // if already found a jpeg, close it
            if (img != NULL) 
            {
                fclose(img);
            }
            
            // save new jpeg
            sprintf(filename, "%03i.jpg", counter);
            
            counter++;
            
            // open jpeg for writing
            img = fopen(filename, "w");
        }
        
        // write jpeg
        if (counter > 0)
            {
                fwrite(buffer, 512, 1, img);
            }
    }

    //close the last jpeg
    if(img != NULL) 
    {
        fclose(img);
    }
    
    // close file
    fclose(file);
    
    // success
    return 0;
}