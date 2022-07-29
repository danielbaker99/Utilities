#include <stdio.h>
#include <stdlib.h>
int difference;
void compare(FILE *file1, FILE *file2, int length, unsigned char*buffer, unsigned char*buffer2) {
int bytes, a, offset = 0;
do{
	if (length >= 4096) bytes = 4096; else bytes = length;
	fread(buffer, bytes, 1, file1);
	fread(buffer2, bytes, 1, file2);
	for (int i = 0; i < bytes; i++)
	{
		a = buffer[i] - buffer2[i];
		if (a > difference || a < -difference)
		{
			printf("offset %d, difference %d\t%x, %x\n", offset + i, a, buffer[i], buffer2[i]);
		}
	}
	length -= bytes;
	offset += bytes;
} while (length);
}

int main(int argc, char *argv[]) {
	if (argc < 7 || argv[1][1] == '?')
	{
		printf("file_compare [file1] [file2] [difference] [file 1 offset] [file 2 offset] [length]\n\n");
		printf("example\nfile_compare \"D:\\\\source\\\\dev\\\\mpeg\\\\iso 13818-2\\\\output\\\\dct_frame0.bin\"^\n ");
		printf("\"C:\\\\msys64\\\\usr\\\\local\\\\dev\\\\iso13818-5\\\\output\\\\dct0.y\"^\n1 0 0 678912\n");
		exit(1);
	}
	char *file1 = argv[1];//"D:\\source\\dev\\mpeg\\iso 13818-2\\output\\dct_frame0.bin",
	char *file2 = argv[2];//"C:\\msys64\\usr\\local\\dev\\iso13818-5\\output\\dct0.y";
	difference = atoi(argv[3]);
	
	int file1_offset = atoi (argv[4]), file2_offset = atoi(argv[5]), length = atoi(argv[6]);//678912;
	unsigned char *buffer, *buffer2;
	buffer = (unsigned char*) malloc(4096);
	buffer2 = (unsigned char*) malloc(4096);
	FILE* f1, *f2;
	errno_t ret;
	ret = fopen_s(&f1, file1, "rb");
	if (ret) { perror("file open 1"); exit(1);}
	ret = fopen_s(&f2, file2, "rb");
	if (ret) {perror("file open 2"); exit(1);}
	fseek(f1, file1_offset, SEEK_SET);
	fseek(f2, file2_offset, SEEK_SET);
	compare(f1, f2, length, buffer, buffer2);
	fclose(f1);
	fclose(f2);
	return 0;
}

