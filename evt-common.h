#ifndef EVT_COMMON_H
#define EVT_COMMON_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <wchar.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BYTE uint8_t
#define CBYTE const uint8_t
#define WORD uint16_t
#define DWORD uint32_t
#define WCHAR wchar_t
#define UNDEF_DWORD 0xFFFFFFFF
#define UNDEF_WORD 0xFFFF


extern CBYTE SIGNATURE_RECORD[];
extern const size_t SIGNATURE_RECORD_SIZE;

extern CBYTE SIGNATURE_HEADER_START[];
extern CBYTE SIGNATURE_HEADER_END[];

extern CBYTE SIGNATURE_FOOTER_START[];
extern CBYTE SIGNATURE_FOOTER_END[];

extern const size_t HEADER_SIZE;
extern const size_t FOOTER_SIZE;

extern const size_t PADDING_SIZE;

/**
 * @brief Возвращаемые функциями статусы.
 */
typedef enum
{
    StatusOK = 0,       /* статус успеха */
    StatusError = 1,    /* статус ошибки */
    StatusStop = 2,     /* статус корректного выхода из алгоритма до его завершения */
} Status;


/**
 * @brief структура заголовка события.
 */
typedef struct
{
    DWORD length;
    DWORD signature;
    DWORD recordNumber;
    DWORD timeGenerated;
    DWORD timeWritten;
    DWORD eventID;
    WORD  eventType;
    WORD  numStrings;
    WORD  eventCategory;
    WORD  reservedFlags;
    DWORD closingRecordNumber;
    DWORD stringOffset;
    DWORD userSidLength;
    DWORD userSidOffset;
    DWORD dataLength;
    DWORD dataOffset;

    DWORD recoveredLength;
    DWORD recoveredDataLength;
} EventLogRecordHeader;

/**
 * @brief Структура записи события.
 */
typedef struct
{
    EventLogRecordHeader recordHeader;

    WCHAR* sourceName;
    WCHAR* computerName;
    WCHAR* strings;
    BYTE*  data;

    bool isHidden;
} ParsedRecord;

/**
 * @brief Инициализация элемента с типом ParsedFooter:
 *  заполнение всех элементов значениями -1 или NULL в зависимости от типа.
 */
ParsedRecord initParsedRecord();

/**
 * @brief Печать в поток вывода сущность типа parsedRecord с учетом типов и наличия элементов.
 */
void printParsedRecord(const ParsedRecord* record);

/**
 * @brief Структура заголовка журнала.
 */
typedef struct
{
    DWORD headerSize;
    DWORD signature;
    DWORD majorVersion;
    DWORD minorVersion;
    DWORD startOffset;
    DWORD endOffset;
    DWORD currentRecordNumber;
    DWORD oldestRecordNumber;
    DWORD fileSize;
    DWORD flags;
    DWORD retention;
    DWORD endHeaderSize;

    DWORD recoveredCurrentRecordNumber;
    DWORD recoveredOldestRecordNumber;
    DWORD recoveredFileSize;
} ParsedHeader;

/**
 * @brief Инициализация элемента с типом ParsedFooter:
 *  заполнение всех элементов значениями -1 или NULL в зависимости от типа.
 */
ParsedHeader initParsedHeader();

/**
 * @brief Терминальная структура журнала.
 */
typedef struct
{
    DWORD size;
    DWORD one;
    DWORD two;
    DWORD three;
    DWORD four;
    DWORD beginRecord;
    DWORD endRecord;
    DWORD currentRecordNumber;
    DWORD oldestRecordNumber;
    DWORD recordSizeEnd;

    DWORD recoveredCurrentRecordNumber;
    DWORD recoveredOldestRecordNumber;
} ParsedFooter;

/**
 * @brief Инициализация элемента с типом ParsedFooter:
 *  заполнение всех элементов значениями -1 или NULL в зависимости от типа.
 */
ParsedFooter initParsedFooter();

/**
 * @brief Структура динамического массива, содержащего записи событий.
 */
typedef struct
{
  ParsedRecord* records;
  size_t recordsCount;
  size_t capacity;
} RecordsArray;

/**
 * @brief initArray
 * @param arr
 * @param initialCapacity
 * @return
 */
Status initArray(RecordsArray *arr, size_t initialCapacity);

Status initEmptyArray(RecordsArray *arr);

Status insertArray(RecordsArray *arr, ParsedRecord element);

void freeArray(RecordsArray *arr);

/**
 * @brief Структура журнала событий.
 */
typedef struct
{
    bool hasHeader;
    bool hasFooter;
    ParsedHeader header;
    RecordsArray recordsArray;
    ParsedFooter footer;
} EventLog;

/**
 * @brief Печать в поток вывода информации о записях в журнале событий.
 * @param[in] eventLog журнал событий, содержащий записи
 */
void printEventLogRecords(const EventLog* eventLog);

/**
 * @brief Печать в поток вывода информаци из заголовка и треминальной структуры журнала событий.
 * @param[in] eventLog журнал событий, содержащий заголовок и терминальную структуру
 */
void printHeaderFooter(const EventLog* eventLog);

#endif /* EVT_COMMON_H */
