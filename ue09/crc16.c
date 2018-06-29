#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

#define CRC16 0x8005

struct stream
{
	FILE* file;
	char cur_byte;
	unsigned int cur_bitindex;
	
	int16_t trailer;
	unsigned int trailer_index;

	long seekend;
};

unsigned int next(struct stream* bs)
{
	unsigned int shiftop;
	unsigned int bit;

	if (feof(bs->file) || ftell(bs->file) >= bs->seekend)
	{
		shiftop = 15 - bs->trailer_index;
		bit = (bs->trailer & (1 << shiftop)) >> shiftop;

		bs->trailer_index = (bs->trailer_index + 1) % 16;
		return bit;
	}
	else
	{
		shiftop = 15 - bs->cur_bitindex;
		bit = (bs->cur_byte & (1 << shiftop)) >> shiftop;
		
		bs->cur_bitindex = (bs->cur_bitindex + 1) % 16;

		if (!bs->cur_bitindex)
		{
			bs->cur_byte = fgetc(bs->file);
		}

		return bit;
	}
}

uint16_t crc(FILE* fp, int16_t trailer, int verify)
{	
	struct stream bs;
	int16_t cur_remainder;

	bs.file = fp;
	bs.cur_bitindex = 0;
	bs.trailer = trailer;
	bs.trailer_index = 1;

	fseek(bs.file, verify ? -2:0, SEEK_END);
	bs.seekend = ftell(bs.file);
	rewind(bs.file);

	cur_remainder = fgetc(bs.file);

	if (!feof(bs.file))
	{
		cur_remainder <<= 8;
		cur_remainder |= fgetc(bs.file);
	}

	bs.cur_byte = fgetc(bs.file);

	while (1)
	{
		//shift until MSB is 1
		while (!(cur_remainder & 0x8000))
		{
			if (!bs.trailer_index)
			{
        // back to start of file
				rewind(bs.file);
				return cur_remainder;
			}

			cur_remainder <<= 1;
			cur_remainder |= next(&bs);
		}

		cur_remainder ^= CRC16;
	}
}

int main(int argc, char* argv[])
{
	if (argc != 2)
		return 1;

	char* crcextension = ".crc";

	char* filename = argv[1];
	char* extension = strrchr(filename, '.');
	FILE* file = fopen(filename, "r");

	FILE* newfile;
	char* newfilename;

	char buf[256];

	int16_t checksum;

	if (!file) {
    printf("File not found.\n");
		return 1;
	}

	if (!strcmp(extension, crcextension))
	{
		fseek(file, -2, SEEK_END);
		fread(&checksum, 1, 2, file);

		if (!crc(file, checksum, 1))
		{
			long seekend;

			*extension = '\0';
			newfilename = filename;
			newfile = fopen(newfilename, "w");

			fseek(file, -2, SEEK_END);
			seekend = ftell(file);
			rewind(file);

			while (ftell(newfile) < seekend)
			{
				fwrite(buf, 1, fread(buf, 1, 1, file), newfile);
			}

      // close file
			fclose(newfile);
		}
		else
		{
			printf("Checksum and file contents don't match\n");
		}

		fclose(file);
	}
	else
	{
		checksum = crc(file, 0, 0);

		printf("Generated checksum: 0x%04x\n", checksum);

    // create new filename
		newfilename = malloc(strlen(filename) + strlen(crcextension) + 1);
		strcpy(newfilename, filename);
		strcat(newfilename, crcextension);

    // create new file
		newfile = fopen(newfilename, "w");

		while (!feof(file))
		{
			fwrite(buf, 1, fread(buf, 1, 256, file), newfile);
		}
		//append checksum to new .crc file
		fwrite(&checksum, 1, 2, newfile);

    // close files
		fclose(file);		
		fclose(newfile);
	}

	return 0;
}
