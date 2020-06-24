// ConsoleApplication10.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;

#pragma pack(2)
typedef struct {
    int32_t chunkId;   // Завжди містить значення 0x52494646 (літери "RIFF")
    int32_t chunkSize; // 36 + розмір другого підрозділу в байтах
                       // Іншими словами 4 + (8 + SubChunk1Size) + (8 + SubChunk2Size)
                       // Це розмір всього файла мінус 8 байтів які займають поля chunkId та chunkSize
    int32_t format;    // Для wav-файла це завжди 0x57415645 (літери "WAVE")
} RIFFHEADER;

#pragma pack(2)
typedef struct {
    int32_t subchunk1Id;   // Завжди 0x666d7420 – літери "fmt "
    int32_t subchunk1Size; // Завжди 16 для аудіо PCM. Це розмір частини підрозділу, що слідує після цього числа
    int32_t audioFormat;   // PCM = 1
    int16_t numChannels;   // Mono = 1, Stereo = 2
    int32_t sampleRate;    // Наприклад 44100
    int32_t byteRate;      // == SampleRate * NumChannels * BitsPerSample/8
    int32_t blockAlign;    // == NumChannels * BitsPerSample/8
    int16_t bitsPerSample; // 8 bits = 8, 16 bits = 16, etc.
} SUBCHUNK1;

#pragma pack(2)
typedef struct {
    int32_t subchunk2Id;   // 0x64617461 – літери "data"
    int32_t subchunk2Size; // ==  * NumChannels * BitsPerSample/8, кількість байтів аудіоданих
    int8_t* data;         // семпли
} SUBCHUNK2; 


RIFFHEADER head;
SUBCHUNK1 sub1;
SUBCHUNK2 sub2;
int numSamples,sampleSize;

int main()
{
   // cout << "Enter path to audiofile\n";
	//char filename[256];
	//gets_s(filename);
	FILE* baseFile = fopen("input.wav", "rb");
    FILE* writeFile = fopen("out.wav", "wb");
 // cout << "Enter number to scale\n";
  //  int n;
 // cin>>n;
    fread(&head,sizeof(head),1,baseFile);
    fread(&sub1,sizeof(sub1),1,baseFile);
    fread(&sub2.subchunk2Id,sizeof(int32_t),1,baseFile);
    fread(&sub2.subchunk2Size,sizeof(int32_t),1,baseFile);
    sampleSize = sub1.bitsPerSample / 8;
    numSamples = sub2.subchunk2Size * 8 / sub1.bitsPerSample;
    sub2.data = new int8_t[numSamples];
    for(int i = 0;i<numSamples;i+=1)
        fread(&sub2.data[i],sizeof(int8_t),1,baseFile);

    fwrite(&head, sizeof(head), 1, writeFile);
    fwrite(&sub1, sizeof(sub1), 1, writeFile);
    fwrite(&sub2.subchunk2Id, sizeof(int32_t), 1, writeFile);
    fwrite(&sub2.subchunk2Size, sizeof(int32_t), 1, writeFile);
    for (int i = 0; i < numSamples; i += 1) {
        fwrite(&sub2.data[i], sizeof(int8_t), 1, writeFile);
    }
   

}
