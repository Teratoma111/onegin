#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cassert>
#include <ctype.h>

struct line
{
	int length = 0;
	char* beginning_of_string = 0;
};

size_t read_from_file(const char* name, char** buffer);
size_t getsize(const char* name);
size_t counter_of_lines(char* buffer);
line* prepare_lines(char** buffer, int nstr);
void write_in_file(const char* name, line* lines, int nstr, char const mode[]);
int comparator(const void* l1, const void* l2);
int reverse_comparator(const void* l1, const void* l2);
void write_in_file(const char* name, char** buffer, int sizeoffile, const char mode[]);


int main()
{
	char* buffer = NULL;
	const char name[11] = "gamlet.txt";
    const char name_out[8] = "out.txt";
	size_t readed = read_from_file(name, &buffer);	
	
	size_t nstr = counter_of_lines(buffer);
	
	line* lines = prepare_lines(&buffer, nstr);
		
	qsort(lines, nstr, sizeof(line), comparator);
	write_in_file(name_out, lines, nstr, "w");	
	qsort(lines, nstr, sizeof(line), reverse_comparator);
	write_in_file(name_out, lines, nstr, "a+");
	
	write_in_file(name_out, &buffer, readed, "a+");
}
size_t read_from_file(const char* name, char** buffer)
{
	assert(name != NULL);
	assert(buffer != NULL);
	
	size_t sizeoffile = getsize(name);

	*buffer = (char*) calloc(sizeof(char*), sizeoffile + 1);

	FILE* file_with_onegin = fopen(name, "rb");
	
	size_t readed = fread(*buffer, sizeof(char), sizeoffile, file_with_onegin);
	
	fclose(file_with_onegin);
	
	return readed;
}

size_t getsize(const char* name)
{
	assert(name != NULL);
	
	FILE* gamlet = fopen(name, "rd");
	
	fseek(gamlet, 0, SEEK_END);
	
	size_t sizeoffile = ftell (gamlet);
	
	rewind (gamlet); 
	fclose(gamlet);
	return sizeoffile;
}
size_t counter_of_lines(char* buffer)
{
	assert(buffer != NULL);	
	
	size_t nstr = 0;
	char* temporary = buffer;

	while(true)
	{
		temporary = strchr(temporary, '\n');
		if(temporary == nullptr)
			break;
		temporary++;
		++nstr;
	}
	
	return nstr;
}
line* prepare_lines(char** buffer, int nstr)
{
	assert(buffer != NULL);

	line* real_lines = (line *) calloc(nstr, sizeof(line));
	line* lines = real_lines;
	char* point = strchr(*buffer, '\n');
	*point = '\0';
	lines -> beginning_of_string = *buffer;
	lines -> length = point - *buffer + 1;
	lines++;
	char* old_point = *buffer;
	
	while (true)
	{
		old_point = point + 1;
		point = strchr(point + 1, '\n');
		
		if (point == NULL)
			break;
			
		*point = '\0';
		
		lines -> length = point - old_point;
		
		lines -> beginning_of_string = old_point;
		
		lines++;	
	}

	return real_lines;
}
void write_in_file(const char* name, line* lines, int nstr, const char mode[])
{
	assert(name != NULL);
	assert(lines != NULL);
	
	FILE* file_with_sort_onegin = fopen(name, mode);
	
	for(int i = 0; i < nstr; ++i)
	{
		if(*(lines[i].beginning_of_string) != '\0')
		{
			fprintf(file_with_sort_onegin, "%s\n", lines[i].beginning_of_string);
		}
	}
	fprintf(file_with_sort_onegin, "\n\n");
	fclose(file_with_sort_onegin);
}




void write_in_file(const char* name, char** buffer, int readed, 
							const char mode[])
{
	assert(name != NULL);
	assert(buffer != NULL);
	
	FILE* file_with_sort_onegin = fopen(name, mode);
	for(int i = 0; i < readed; ++i)
	{
		if((*buffer)[i] != '\0')
		{
			fprintf(file_with_sort_onegin, "%c", 
					(*buffer)[i]);
		}
		else
		{
			fprintf(file_with_sort_onegin, "\n");
		}
	}
	fclose(file_with_sort_onegin);
}




int comparator(const void* l1, const void* l2)
{
	const char* a = ((line*) l1) -> beginning_of_string;
	const char* b = ((line*) l2) -> beginning_of_string;
	
	return strcmp(a, b);
}
int reverse_comparator(const void* l1, const void* l2)
{
	line* line1 = ((line*) l1);
	line* line2 = ((line*) l2);
	
	 
	 char* pointer1 = line1 -> beginning_of_string + line1 -> length;
	 char* pointer2 = line2 -> beginning_of_string + line2 -> length;
	 
	 while(true)
	 {
	 	if(!isalpha(*pointer1))
	 	{
	 		--pointer1;
	 		continue;
	 	}
	 	
	 	if(!isalpha(*pointer2))
	 	{
	 		--pointer2;
	 		continue;
	 	}
	 	
	 	if ((*pointer1 == *pointer2 - 32) || 
			(*pointer1 == *pointer2 + 32))
		{
			--pointer1;
			--pointer2;
		}
	 	
	 	if ((*pointer1 > *pointer2) || 
	 		(pointer2 <= (line2 -> beginning_of_string) && pointer1 > (line1 -> beginning_of_string)))
	 		return 1;
	 		
	 	if ((*pointer2 > *pointer1) || 
	 		(pointer1 <= (line1 -> beginning_of_string) && pointer2 > (line2 -> beginning_of_string)))
	 		return -1;
	 	
	 	--pointer1;
	 	--pointer2;
	 	
	 }
	 return 0;
	 
}