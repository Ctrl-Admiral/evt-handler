#ifndef EVT_HEADER_H
#define EVT_HEADER_H

//#include <common.h>
#include <sys/types.h>
#include <stdint.h>
#include <libevt.h>>

typedef struct evt_file_header evt_file_header_t;

struct evt_file_header
{
    /* (Header) size
     */
    uint8_t size[ 4 ];

    /* Signature
     */
    uint8_t signature[ 4 ];

    /* Major format version
     */
    uint8_t major_format_version[ 4 ];

    /* Minor format version
     */
    uint8_t minor_format_version[ 4 ];

    /* First (oldest) record offset
     */
    uint8_t first_record_offset[ 4 ];

    /* End of file record offset
     */
    uint8_t end_of_file_record_offset[ 4 ];

    /* Last (newest) record number
     */
    uint8_t last_record_number[ 4 ];

    /* First (oldest) record number
     */
    uint8_t first_record_number[ 4 ];

    /* Maximum file size
     */
    uint8_t maximum_file_size[ 4 ];

    /* File flags
     */
    uint8_t file_flags[ 4 ];

    /* Retention
     */
    uint8_t retention[ 4 ];

    /* Copy of (header) size
     */
    uint8_t copy_of_size[ 4 ];
};
#endif // EVT_HEADER_H
