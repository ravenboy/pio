

#ifndef _SD_LIBRARY_H
#define _SD_LIBRARY_H

enum SDFat_eLEN
{
    eLen_PREFS_FILE_PATH = 40,
    eLEN_MAX_PATH_LENGTH= 100,
};  

//maxSck can be  SPI_HALF_SPEED, SPI_FULL_SPEED etc
extern bool Start_SD(const uint8_t chipSelect, bool permLoopIfFail = true, uint32_t maxSck = SPI_HALF_SPEED);

extern void DoFormat(void);
extern void DoInteractiveFormat(void);
extern void DoSDInfo(bool interactive = true);
extern void DoSDBenchmark(bool interactive = true);
extern void DoSDDirFuncs(bool interactive = true);
extern void DoSDOpenNext(bool interactive = true);
extern void DoSDQuickStart(void);
extern void SendSDFatState(void);

extern bool SetupFile(const char* dirName, const char* fileName); 
#endif //_SD_LIBRARY_H
