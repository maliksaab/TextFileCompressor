/*
author : taimoor haider aslam
roll # : l14-4043
section: cs-d spring 2015
*/

/********************************************************/
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS	 // This block of code is used to keep the _CRT_SECURE_NO_WARNINGS disable
#endif				// otherwise fopen function used for command line arguments will not work
/********************************************************/


#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

using namespace std;


void readinfofile(char c[], int  &bits, int &bytes_required);
void readcmpfile(char filename[], char * cstream, int bytes_required);
void unpackcstream(unsigned char *cstream, char *stream, int bytes_required);
void char_cstrm_2_unsig_char_cstrm(char *Cstream, unsigned char *cstream, int bytes_required);
void fill_binary_of_c(char binary_of_c[][8], int bits, int uniqchars);
void decodingbinary(int bits, int totalcharcount, char c[], char *stream, char filename[]);

int main(int argc, char * argv[])
{
	 if (argc != 2) /* argc should be 2 for correct execution */
	 {
		  cout << "please enter correct arguments or filename with complete extensions\n\n";
	 }
	 else
	 {

		  int  bits;
		  int  bytes_required;
		  int totalcharcount = 0;
		  int uniqchars = 0;
		  char c[255] = { 0 };
		  char filename[100] = "\0";
		  char binary_of_c[255][8] = { 0 };

		  strcpy(filename, argv[1]);  //copy argv[1] in filename 

		  FILE *file = fopen(filename, "r");


		  readinfofile(c, bits, totalcharcount);	   //this function will read data from hidden info.txt file 

		  bytes_required = ceil(double(totalcharcount*bits) / 8);

		  char* Cstream = new char[bytes_required]; // will be copied to unsigned char latter
		  Cstream[0] = 0;

		  readcmpfile(filename, Cstream, bytes_required);

		  unsigned char * cstream = new unsigned char[bytes_required];
		  cstream[0] = 0;

		  char_cstrm_2_unsig_char_cstrm(Cstream, cstream, bytes_required);

		  delete[]Cstream;

		  char * stream = new char[totalcharcount*bits];
		  stream[0] = 0;

		  unpackcstream(cstream, stream, bytes_required); //after execution of this function, stream[] will have all the 0's & 1's

		  decodingbinary(bits, totalcharcount, c, stream,filename);
		  delete[]cstream;

	 }
}

void readinfofile(char c[], int  &bits, int &totalcharcount)
{
	 char reader = 0;
	 ifstream fin("info.txt",ios::out);

	 if (fin.is_open())
	 {
		  fin >> bits >> totalcharcount;
		  fin.ignore(1, ' ');
		  for (int i = 0; (fin.get(reader)); i++)
		  {
			   c[i] = reader;
		  } 
	 }
	 else
	 {
		  cout << "\nfailed to open info.txt \n\n";
	 }
}

void readcmpfile(char filename[], char * cstream, int bytes_required)
{
	 ifstream fin(filename, ios::in | ios::binary);
	 
	 if (fin.is_open())
	 {
		  fin.read(cstream,bytes_required);
	 }
	 else
	 {
		  cout << "\nfailed to open "<<filename<<"\n\n";
	 }
}

void char_cstrm_2_unsig_char_cstrm(char *Cstream,unsigned char *cstream,int bytes_required)
{
	 for (int i = 0; i < bytes_required; i++)
	 {
		  cstream[i] = Cstream[i];
	 }
}

void unpackcstream(unsigned char *cstream, char *stream, int bytes_required)
{
	 unsigned char ans = 0x00;
	 unsigned char mask = 0x80;
	 int k = 0, j = 0;

	 while (bytes_required!=0)	 //controlling bytes
	 {
		  for (int i = 0; i < 8; i++)  //controlling bits
		  {
			   ans = mask & cstream[j];
			   if (ans != 0)
			   {
				    stream[k] = '1';
			   }
			   else
			   {
				    stream[k] = '0';
			   }
			   if(i<7)
			   mask = mask >> 1;

			   k++;
		  }
		  j++;
		  mask = 0x80;
		  bytes_required--;
	 }
}

void fill_binary_of_c(char binary_of_c[][8], int bits, int uniqchars)
{
	 char mask = 1;				
	 int  is_nonzero = bits- 1;		
	 for (int i = 0; i < uniqchars; i++)
	 {
		  for (int j = 0; j < bits; j++)
		  {
			   mask = mask << is_nonzero;		    
			   mask = mask & i;
			   if (mask != 0)
			   {
				    binary_of_c[i][j] = '1';
			   }
			   else
			   {
				    binary_of_c[i][j] = '0';
			   }
			   is_nonzero--;
			   mask = 1;
		  }
		  is_nonzero = bits - 1;
	 }
}

void decodingbinary(int bits, int totalcharcount, char c[], char *stream, char filename[])
{
	 int len = strlen(filename);
	 filename[len - 3] = 't';
	 filename[len - 2] = 'x';
	 filename[len - 1] = 't';
	 ofstream fout(filename);

	 char  mask = 1;
	 char ans = 0;
	 int  shift = bits -1;
	 for (int i = 0,j = 0; i < totalcharcount; i++)
	 {
		  for (int k = 0; k < bits;k++, j++)
		  {
			   if (stream[j] == '0')
			   {
				    mask = 0;
				    mask = mask << shift;
				    ans = ans | mask;
			   }
			   else
			   {
				    mask = 1;
				    mask = mask << shift;
				    ans = ans | mask;
			   }
			   shift--;
		  }
		  fout << c[ans];
		  mask = 1;
		  ans = 0;
		  shift = bits - 1;
	 }
}
