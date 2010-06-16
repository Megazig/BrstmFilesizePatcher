#include <iostream>
#include <vector>
#include <string>
//#include <sstream>
//#include <map>
//#include <cstdlib>

#include <iostream>
#include <fstream>

#include <string.h>

//#include <stdio.h>
#include <stdlib.h>
//#include <arpa/inet.h>

using namespace std;

#define DEBUG 1

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;

#define be16(x)        ((x>>8)|(x<<8))
#define be32(x)        ((x>>24)|((x<<8)&0x00FF0000)|((x>>8)&0x0000FF00)|(x<<24))
#define be64(x)        ((x>>56)|((x<<40)&(u64)0x00FF000000000000)|((x<<24)&(u64)0x0000FF0000000000)|((x<<8)&(u64)0x000000FF00000000)|((x>>8)&(u64)0x00000000FF000000)|((x>>24)&(u64)0x0000000000FF0000)|((x<<40)&(u64)0x000000000000FF00)|(x<<56))

u16 Big16( char * pointer )
{
	return be16(*(u16*)(pointer));
}

u32 Big32( char * pointer )
{
	return be32(*(u32*)(pointer));
}

u32 Big32( const u32 * pointer )
{
	return be32(*pointer);
}

u32 Read32( char * pointer )
{
	return *(u32*)pointer;
}

bool Write32( char * pointer , u32 val )
{
	if(pointer)
		*(u32*)pointer = be32(val);
	else
		return false;
	return true;
}

string nullterm(char* buffer) {
	string blah = buffer;
	return blah;
}

typedef struct
{
	u32 magic;
	u16 endian;
	u16 version;
	u32 filesize;
	u16 headersize;
	u16 chunk_count;
	u32 symb_offset;
	u32 symb_size;
	u32 info_offset;
	u32 info_size;
	u32 file_offset;
	u32 file_size;
	u32 padding[6];
} brsar_header;

void SwapHeader(brsar_header * header)
{
	header->magic = be32(header->magic);
	header->endian = be16(header->endian);
	header->version = be16(header->version);
	header->filesize = be32(header->filesize);
	header->headersize = be16(header->headersize);
	header->chunk_count = be16(header->chunk_count);
	header->symb_offset = be32(header->symb_offset);
	header->symb_size = be32(header->symb_size);
	header->info_offset = be32(header->info_offset);
	header->info_size = be32(header->info_size);
	header->file_offset = be32(header->file_offset);
	header->file_size = be32(header->file_size);
#ifdef DEBUG
	cout << endl;
	cout << "Magic: 0x" << hex << header->magic << endl;
	if ( header->endian == 0xfeff )
		cout << "Endian: BIG" << endl;
	else
		cout << "Endian: LITTLE" << endl;
	cout << "Version: 0x" << hex << header->version << endl;
	cout << "Filesize: 0x" << hex << header->filesize << endl;
	cout << "Header Size: 0x" <<hex<<header->headersize<<endl;
	cout << "Chunk Count: " <<dec<< header->chunk_count <<endl;
	cout << "Symb offset: 0x" << hex << header->symb_offset;
	cout << " size: 0x" << hex << header->symb_size << endl;
	cout << "Info offset: 0x" << hex << header->info_offset;
	cout << " size: 0x" << hex << header->info_size << endl;
	cout << "File offset: 0x" << hex << header->file_offset;
	cout << " size: 0x" << hex << header->file_size << endl;
#endif
}

typedef struct
{
	u32 sounds_ref;
	u32 sounds;
	u32 banks_ref;
	u32 banks;
	u32 players_ref;
	u32 players;
	u32 files_ref;
	u32 files;
	u32 groups_ref;
	u32 groups;
	u32 unknown_ref;
	u32 unknown;
} info_offsets;

void SwapInfoOffsets(info_offsets * offsets)
{
	offsets->sounds_ref = be32(offsets->sounds_ref);
	offsets->sounds = be32(offsets->sounds);
	offsets->banks_ref = be32(offsets->banks_ref);
	offsets->banks = be32(offsets->banks);
	offsets->players_ref = be32(offsets->players_ref);
	offsets->players = be32(offsets->players);
	offsets->files_ref = be32(offsets->files_ref);
	offsets->files = be32(offsets->files);
	offsets->groups_ref = be32(offsets->groups_ref);
	offsets->groups = be32(offsets->groups);
	offsets->unknown_ref = be32(offsets->unknown_ref);
	offsets->unknown = be32(offsets->unknown);
#ifdef DEBUG
	cout << endl;
	cout << "Info Offsets" << endl;
	cout << "sounds ref: 0x" << hex << offsets->sounds_ref <<
		" offsets: 0x" << hex << offsets->sounds << endl;
	cout << "banks ref: 0x" << hex << offsets->banks_ref <<
		" offsets: 0x" << hex << offsets->banks << endl;
	cout << "players ref: 0x" << hex << offsets->players_ref <<
		" offsets: 0x" << hex << offsets->players << endl;
	cout << "files ref: 0x" << hex << offsets->files_ref <<
		" offsets: 0x" << hex << offsets->files << endl;
	cout << "groups ref: 0x" << hex << offsets->groups_ref <<
		" offsets: 0x" << hex << offsets->groups << endl;
	cout << "unknown ref: 0x" << hex << offsets->unknown_ref <<
		" offsets: 0x" << hex << offsets->unknown << endl;
#endif
}

typedef struct
{
	u32 filesize;	// headerLen when not external
	u32 dataLen;	// 0 when external
	u32 magic;		// 0xffffffff
	u32 stringRef;
	u32 stringOff;
	u32 listRef;
	u32 listOff;
} info_file_info;

void SwapInfoFileInfo(info_file_info * info)
{
	info->filesize = be32(info->filesize);
	info->dataLen = be32(info->dataLen);
	info->magic = be32(info->magic);
	info->stringRef = be32(info->stringRef);
	info->stringOff = be32(info->stringOff);
	info->listRef = be32(info->listRef);
	info->listOff = be32(info->listOff);
#ifdef DEBUG
	cout << endl;
	cout << "Filesize: 0x" << hex << info->filesize << endl;
	cout << "Data Length: 0x" << hex << info->dataLen << endl;
	cout << "Magic: 0x" << hex << info->magic << endl;
	cout << "StrngRef: 0x" << hex << info->stringRef;
	cout << " StrngOff: 0x" << hex << info->stringOff << endl;
	cout << "ListRef 0x" << hex << info->listRef;
	cout << " ListOff: 0x" << hex << info->listOff << endl;
#endif
}

bool PatchBrsar(char* buffer, string a, u32 a_len)
{
	brsar_header rsar;
	memcpy(&rsar, buffer, sizeof(brsar_header));
	SwapHeader(&rsar);

	info_offsets i_offsets;
	memcpy(&i_offsets, buffer + rsar.info_offset + 8,
			sizeof(info_offsets));
	SwapInfoOffsets(&i_offsets);

	u32 info_sounds_count = Big32(buffer + rsar.info_offset
			+ 8 + i_offsets.sounds);
#ifdef DEBUG
	cout << "Info Sounds count: 0x" << hex << info_sounds_count
		<< "(" << dec << info_sounds_count << ")" << endl;
#endif

	u32 info_files_count = Big32(buffer + rsar.info_offset
			+ 8 + i_offsets.files);
#ifdef DEBUG
	cout << "\nInfo Files count: 0x" << hex << info_files_count
		<< "(" << dec << info_files_count << ")" << endl;
#endif
	vector< pair<u32,u32> > file_offsets;
	for(int ii=0; ii<info_files_count; ii++) {
		char* offs = buffer + rsar.info_offset + 8 + 
			i_offsets.files + 4;
		u32 ref = Big32(offs + ii*8);
		u32 off = Big32(offs + ii*8 + 4);
		file_offsets.push_back(pair<u32,u32>(ref,off));
#ifdef DEBUG
		u32 temp_offs = rsar.info_offset+8+off;
		cout << "\tref: " << hex << ref <<
			" offset: " << hex << off <<
			"(0x" << hex << temp_offs << ")" << endl;
#endif
	}
	for(int ii=0; ii<file_offsets.size(); ii++) {
		u32 temp_offs = rsar.info_offset + 8 +
			file_offsets[ii].second;
		info_file_info i_f_info;
		memcpy(&i_f_info, buffer+temp_offs,
				sizeof(info_file_info));
		SwapInfoFileInfo(&i_f_info);
		u32 strOff = rsar.info_offset + 8 + i_f_info.stringOff;
		u32 listOff = rsar.info_offset + 8 + i_f_info.listOff;
#ifdef DEBUG
		cout << "\tstringOffset: 0x" << hex << strOff << endl;
		cout << "\tlistOffset:   0x" << hex << listOff << endl;
#endif
		string info_file_name;
		if((!i_f_info.dataLen)&&(i_f_info.stringOff)) {
			info_file_name = nullterm(buffer+strOff);
			string temp_name = info_file_name;
			int pos = temp_name.rfind("/");
			if(pos != string::npos)
				temp_name = temp_name.substr(pos+1);
			if(a == temp_name) {
#ifdef DEBUG
				cout << "patching no. " << dec<<ii << endl;
#endif
				Write32(buffer+temp_offs, a_len);
			}
#ifdef DEBUG
			cout << "             " << temp_name << endl;
			cout << "testing for: " << a << endl;
#endif
		}
	}

	return true;
}

void ShowMessage(char * app_name)
{
	cout << app_name << " v1.0" << endl;
	cout << "by megazig" << endl;
}

int main(int argc, char ** argv)
{
	if(argc == 1) {
		cout << "Usage: " << argv[0] << " <brstm> <brsar>";
		cout << endl;
		return EXIT_FAILURE;
	}
	ShowMessage(argv[0]);

	ifstream myBrstm(argv[1], ios::in);
	if ( !myBrstm )
	{
		cout << "File ";
		cout << argv[1] << "could not be opened" << endl;
		return EXIT_FAILURE;
	}
	myBrstm.seekg(0, ifstream::end);
	string brstm_name = argv[1];
	u32 brstm_len = myBrstm.tellg();
	myBrstm.seekg(0);		// not needed
	myBrstm.close();

#ifdef DEBUG
	cout << "Brstm size: 0x" << hex << brstm_len << endl;
#endif

	ifstream myBrsar(argv[2], ios::in);
	if ( !myBrsar )
	{
		cout << "File ";
		cout << argv[2] << "could not be opened" << endl;
		return EXIT_FAILURE;
	}
	myBrsar.seekg(0, ifstream::end);
	string brsar_name = argv[2];
	u32 myBrsarSize = myBrsar.tellg();
	myBrsar.seekg(0);

	char * buffer = new char[myBrsarSize];
	myBrsar.read(buffer, myBrsarSize);
	myBrsar.close();
#ifdef DEBUG
	cout << "Brsar size: 0x" << hex << myBrsarSize << endl;
#endif

	// get brsar path and filename
	string brsar_basepath = brsar_name;
	string brsar_basename = brsar_name;
	int pos = brsar_basepath.rfind("/");
	if ( pos != string::npos )
	{
		brsar_basepath = brsar_basepath.substr(0,pos);
		brsar_basename = brsar_basename.substr(pos+1);
	}
	/*
	// remove brsar path from brstm name
	string brstm_basename = brstm_name;
	string brstm_basepath = brstm_name;
	if(brstm_name.rfind(brsar_basepath) != string::npos)
	{
#ifdef DEBUG
		cout << "brsar path found in brstrm" << endl;
#endif
		brstm_basename = brstm_name.substr(brsar_basepath.size()+1);
	}
	*/
	string brstm_basename = brstm_name;
	string brstm_basepath = brstm_name;
	pos = brstm_basepath.rfind("/");
	if ( pos != string::npos )
	{
		brstm_basepath = brstm_basepath.substr(0,pos);
		brstm_basename = brstm_basename.substr(pos+1);
	}

	PatchBrsar(buffer, brstm_basename, brstm_len);
	ofstream patched_file(argv[2], ios::out);
	if ( !patched_file )
	{
		cout << "File ";
		cout << argv[2] << "could not be patched" << endl;
		return EXIT_FAILURE;
	}
	patched_file.write(buffer, myBrsarSize);
	patched_file.close();

	delete [] buffer;
	return EXIT_SUCCESS;
}
