#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstring>
using namespace std;
//Wav Header
struct wav_header_t
{
    char chunkID[4]; //"RIFF" = 0x46464952
    unsigned long chunkSize; //28 [+ sizeof(wExtraFormatBytes) + wExtraFormatBytes] + sum(sizeof(chunk.id) + sizeof(chunk.size) + chunk.size)
    char format[4]; //"WAVE" = 0x45564157
    char subchunk1ID[4]; //"fmt " = 0x20746D66
    unsigned long subchunk1Size; //16 [+ sizeof(wExtraFormatBytes) + wExtraFormatBytes]
    unsigned short audioFormat;
    unsigned short numChannels;
    unsigned long sampleRate;
    unsigned long byteRate;
    unsigned short blockAlign;
    unsigned short bitsPerSample;
    //[WORD wExtraFormatBytes;]
    //[Extra format bytes]
};

//Chunks
struct chunk_t
{
    char ID[4]; //"data" = 0x61746164
    unsigned long size;  //Chunk data bytes
};

void WavReader(const char* fileName, const char* fileToSave,int n)
{
    /*
        READ DATA FROM FILE
    */
    FILE* fin = fopen(fileName, "rb");
    wav_header_t header,newHeader;
    fread(&header, sizeof(header), 1, fin);
    chunk_t chunk,newChunk;
    while (true)
    {
        fread(&chunk, sizeof(chunk), 1, fin);
        printf("%c%c%c%c\t" "%li\n", chunk.ID[0], chunk.ID[1], chunk.ID[2], chunk.ID[3], chunk.size);
        if (*(unsigned int*)&chunk.ID == 0x61746164)
            break;
        fseek(fin, chunk.size, SEEK_CUR);
    }
    int sample_size = header.bitsPerSample / 8;
    int samples_count = chunk.size * 8 / header.bitsPerSample;
    printf("Samples count = %i\n", samples_count);

    short int* value = new short int[samples_count];
    memset(value, 0, sizeof(short int) * samples_count);
    for (int i = 0; i < samples_count; i++)
    {
        fread(&value[i], sample_size, 1, fin);
        int t = value[i];
    }


    cout << sizeof(value);
    /*
        RESIZE FILE
    */
    newHeader = header;
    newChunk = chunk;
    newChunk.size = chunk.size * n;
    newHeader.chunkSize += (newChunk.size - chunk.size);
    short int* newList = new short int[samples_count * n];
    memset(newList, 0, sizeof(short int) * samples_count * n);
    for (int i = 0; i < samples_count; i += 1) {
        for (int j = 0; j < n; j += 1) {
            newList[n*i + j] = value[i];
            int t = newList[n * i + j];
        }
            
        
    }
    newChunk.size = chunk.size * n;
    FILE* fout = fopen(fileToSave, "wb");
    fwrite(&newHeader, sizeof(newHeader), 1, fout);
    fwrite(&newChunk, sizeof(newChunk), 1, fout);
    for (int i = 0; i < samples_count*n; i++)
    {
        fwrite(&newList[i], sample_size, 1, fout);
    }
    fclose(fin);
    fclose(fout);
}

/*
    MAIN PART OF PROGRAM WITH INPUT
*/
int main()
{
    WavReader("input.wav", "out.wav",2);
    return 0;
}
