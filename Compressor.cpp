
/*
Author : Taimoor Haider Aslam
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

int  No_Of_Bits_Required(int uniqChars);
void Fill_Ascii_In_Array(unsigned char asciiList[]);
int  Unique_and_Total_Char_Counter(char C[], unsigned char asciiList[], int &totalCharCounter, char fileName[]);
void fill_Binary_of_UniqChar(char arr[][8], int bits_required, double uniqChars);
void fill_Binary_In_Stream(char C[], char binaryString[][8], int bits_required, int totalCharCounter, char *Stream, char fileName[]);
void packBits( char *cStream, char *Stream, int cStream_bytes_required);
void binaryFiling(char *cStream, int cStream_bytes_required, char fileName[]);
void infoFile(int bits_required, int totalCharCounter, char C[]);



void main(int argc, char *argv[])
{
	 
	 if (argc != 2) /* argc should be 2 for correct execution */
	 {
		  cout << "please enter correct arguments or filename with complete extensions\n\n";
	 }
	 else
	 {
		  int  bits_required;
		  int totalCharCounter = 0;
		  int cStream_bytes_required;
		  char C[255] = { 0 };
		  char binaryString[255][8] = { 0 };
		  char fileName[100] = { 0 };
		  double uniqChars = 0;
		  double cmpRatio = 0;
		  unsigned char asciiList[255] = { '\0' };

		  Fill_Ascii_In_Array(asciiList);

		  strcpy(fileName,argv[1]);  //copy argv[1] in fileName 

		  FILE *file = fopen(fileName, "r");  

		  uniqChars = Unique_and_Total_Char_Counter(C, asciiList, totalCharCounter, fileName);

		  bits_required = No_Of_Bits_Required(uniqChars);

		  fill_Binary_of_UniqChar(binaryString, bits_required, uniqChars);

		  char *Stream = new char[totalCharCounter*bits_required];

		  fill_Binary_In_Stream(C, binaryString, bits_required, totalCharCounter, Stream,fileName);

		  cStream_bytes_required = ceil(double(totalCharCounter*bits_required) / 8);

		  char *cStream = new char[cStream_bytes_required];

		  packBits(cStream, Stream, cStream_bytes_required);

		  delete[] Stream;

		  binaryFiling(cStream, cStream_bytes_required,fileName);

		  delete[] cStream;

		  infoFile(bits_required, totalCharCounter, C);
	 
		  cmpRatio = (1 - (double(cStream_bytes_required) / double(totalCharCounter))) * 100;
		  
		  cout << "Size of Original File: " << totalCharCounter << " Bytes\n";
		  cout << "Bytes ofCompressed File: " << cStream_bytes_required << " Bytes\n";
		  cout << "Compression Ratio: " << cmpRatio << endl;
	 }
}

void Fill_Ascii_In_Array(unsigned char asciiList[])
{
	 for (int i = 0; i < 255; i++)
	 {
		  asciiList[i] = i;						// Filling of Ascii Chars in allAsciiArray[] 
	 }
}

int Unique_and_Total_Char_Counter(char C[], unsigned char asciiList[], int &totalCharCounter, char fileName[])
{
	 char reader;
	 double uniqChar = 0;		//This int uniqChar will be used to store total unique chars in the given .txt 
	 bool stop = false;

	 ifstream fin(fileName);

	 if (fin.is_open())
	 {
		  while (fin.get(reader))
		  {
			   for (int k = 0; (k < 255 && stop == false); k++)
			   {
				    if (asciiList[k] == reader)
				    {							/* In This Part No of Unique Chars are counted and saved in uniqChar*/
						uniqChar++;
						stop = true;
						asciiList[k] = 0;
				    }
			   }
			   stop = false;
			   totalCharCounter++;
		  }
		
	 }
	 else
	 {
		  cout << "Sorry Their is a problem While opening the file "<< fileName<<" U may not Get the Expected Results ";
	 }

	 fin.close();

	 for (int j = 1, k = 0; j < 255; j++)
	 {
		  if (asciiList[j] == 0)
		  {
			   C[k] = j;
			   k++;
		  }
	 }

	 return uniqChar;
}

int No_Of_Bits_Required(int bits)
{
	 bits = ceil(log2(bits));
	 return bits;
}

void fill_Binary_of_UniqChar(char arr[][8], int bits_required, double uniqChars)
{
	 char mask = 1;
	 int  is_NonZero = bits_required - 1;
	 for (int i = 0; i < uniqChars; i++)
	 {
		  for (int j = 0; j < bits_required; j++)
		  {
			   mask = mask << is_NonZero;
			   mask = mask & i;
			   if (mask != 0)
			   {
				    arr[i][j] = '1';
			   }
			   else
			   {
				    arr[i][j] = '0';
			   }
			   is_NonZero--;
			   mask = 1;
		  }
		  is_NonZero = bits_required - 1;
	 }
}

void fill_Binary_In_Stream(char C[], char binaryString[][8], int bits_required, int totalCharCounter, char *Stream, char fileName[])
{
	 int i = 0;
	 int k = 0;
	 char reader;

	 ifstream fin(fileName);

	 if (fin.is_open())
	 {
		  while (fin.get(reader))
		  {
			   while (C[i] != reader)
				    i++;

			   for (int j = 0; j < bits_required; j++)
			   {
				    Stream[k] = binaryString[i][j];
				    k++;
			   }
			   i = 0;
		  }
	 }
	 fin.close();
}

void packBits(char *cStream, char *Stream, int cStream_bytes_required)
{
	 unsigned char ans;
	 unsigned char mask;
	 int i = 0;
	 int j = cStream_bytes_required;
	 int l = 0;
	 while (j != 0)
	 {
		  ans = 0x00;
		  mask = 0x80;
		  for (int k = 0; k < 8; k++)
		  {
			   if (Stream[l] == '1')
			   {
				    ans = ans | mask;
			   }
			   mask = mask >> 1;
			   l++;
		  }
		  cStream[i] = ans;
		  i++;
		  j--;
	 }
}

void binaryFiling(char *cStream, int cStream_bytes_required, char fileName[])
{
	 int i = 0;
	 while (fileName[i]!=0)
		  i++;
	 fileName[i - 3] = 'c';
	 fileName[i - 2] = 'm';
	 fileName[i - 1] = 'p';

	 ofstream fout(fileName, ios::out | ios::binary);
	 if (fout.is_open())
	 {
		  fout.write(cStream, cStream_bytes_required);
	 }
	 else
	 {
		  cout << "Binary Filing failed, Couldn't Write on"<<fileName<<"\n\n";
	 }

	 fout.close();
}

void infoFile(int bits_required, int totalCharCounter, char C[])
{
	 ofstream fout("info.txt");

	 if (fout.is_open())
	 {
		  fout << bits_required <<" "<< totalCharCounter<<" "<<C  ;
	 }
	 else
	 {
		  cout << "\nFailed to Write File info.txt \n\n";
	 }
	 
	 BOOL result = SetFileAttributes(L"info.txt", FILE_ATTRIBUTE_HIDDEN);
	 
	 fout.close();
	 
}
