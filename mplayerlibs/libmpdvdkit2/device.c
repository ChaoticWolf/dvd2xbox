/*****************************************************************************
 * device.h: DVD device access
 *****************************************************************************
 * Copyright (C) 1998-2002 VideoLAN
 * $Id: device.c,v 1.1 2003/11/04 18:28:55 wiso Exp $
 *
 * Authors: St�phane Borel <stef@via.ecp.fr>
 *          Samuel Hocevar <sam@zoy.org>
 *          H�kan Hjort <d95hjort@dtek.chalmers.se>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111, USA.
 *****************************************************************************/

/*****************************************************************************
 * Preamble
 *****************************************************************************/
#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifdef HAVE_UNISTD_H
#   include <unistd.h>
#endif

#if defined( WIN32 )
#   include <io.h>                                                 /* read() */
#else
#   include <sys/uio.h>                                      /* struct iovec */
#endif

#include "dvdcss.h"

#include "common.h"
#include "css.h"
#include "libdvdcss.h"
#include "ioctl.h"
#include "device.h"

#include "Undocumented.h"

#pragma warning (disable:4244)
#pragma warning (disable:4090)
/*****************************************************************************
 * Device reading prototypes
 *****************************************************************************/
static int libc_open  ( dvdcss_t, char const * );
static int libc_seek  ( dvdcss_t, int );
static int libc_read  ( dvdcss_t, void *, int );
static int libc_readv ( dvdcss_t, struct iovec *, int );

#ifdef WIN32
static int win2k_open ( dvdcss_t, char const * );
static int aspi_open  ( dvdcss_t, char const * );
static int win2k_seek ( dvdcss_t, int );
static int aspi_seek  ( dvdcss_t, int );
static int win2k_read ( dvdcss_t, void *, int );
static int aspi_read  ( dvdcss_t, void *, int );
static int win_readv  ( dvdcss_t, struct iovec *, int );

static int aspi_read_internal  ( int, void *, int );
#endif

int _dvdcss_use_ioctls( dvdcss_t dvdcss )
{
#if defined( WIN32 )
    if( dvdcss->b_file )
    {
        return 0;
    }

    /* FIXME: implement this for Windows */
    if( WIN2K )
    {
        return 1;
    }
    else
    {
        return 1;
    }
#else
struct stat fileinfo;
    int ret;

    ret = fstat( dvdcss->i_fd, &fileinfo );
    if( ret < 0 )
    {
        return 1;  /* What to do?  Be conservative and try to use the ioctls */
    }

    /* Complete this list and check that we test for the right things
    * (I've assumed for all OSs that 'r', (raw) device, are char devices
    *  and those that don't contain/use an 'r' in the name are block devices)
    *
    * Linux    needs a block device
    * Solaris  needs a char device
    * Darwin   needs a char device
    * OpenBSD  needs a char device
    * NetBSD   needs a char device
    * FreeBSD  can use either the block or the char device
    * BSD/OS   can use either the block or the char device
    */

    /* Check if this is a block/char device */
    if( S_ISBLK( fileinfo.st_mode ) ||
            S_ISCHR( fileinfo.st_mode ) )
    {
        return 1;
    }
    else
    {
        return 0;
    }
#endif
}

int _dvdcss_open ( dvdcss_t dvdcss )
{
    char psz_debug[200];
    char const *psz_device = dvdcss->psz_device;

    snprintf( psz_debug, 199, "opening target `%s'", psz_device );
    psz_debug[199] = '\0';
    _dvdcss_debug( dvdcss, psz_debug );

#if defined( WIN32 )
    /* If device is not "X:", we are actually opening a file. */

#if defined( _XBOX )

	dvdcss->b_file = 0;

#else
    dvdcss->b_file = !psz_device[0] || psz_device[1] != ':' || psz_device[3];

#endif

    /* Initialize readv temporary buffer */
    dvdcss->p_readv_buffer   = NULL;
    dvdcss->i_readv_buf_size = 0;

    if( !dvdcss->b_file && WIN2K )
    {
        _dvdcss_debug( dvdcss, "using Win2K API for access" );
        dvdcss->pf_seek  = win2k_seek;
        dvdcss->pf_read  = win2k_read;
        dvdcss->pf_readv = win_readv;
        return win2k_open( dvdcss, psz_device );
    }
    else if( !dvdcss->b_file )
    {
        _dvdcss_debug( dvdcss, "using ASPI for access" );
        dvdcss->pf_seek  = aspi_seek;
        dvdcss->pf_read  = aspi_read;
        dvdcss->pf_readv = win_readv;
        return aspi_open( dvdcss, psz_device );
    }
    else
#endif
    {
        _dvdcss_debug( dvdcss, "using libc for access" );
        dvdcss->pf_seek  = libc_seek;
        dvdcss->pf_read  = libc_read;
        dvdcss->pf_readv = libc_readv;
        return libc_open( dvdcss, psz_device );
    }
}

#ifndef WIN32
int _dvdcss_raw_open ( dvdcss_t dvdcss, char const *psz_device )
{
    dvdcss->i_raw_fd = open( psz_device, 0 );

    if( dvdcss->i_raw_fd == -1 )
    {
        _dvdcss_error( dvdcss, "failed opening raw device, continuing" );
        return -1;
    }
    else
    {
        dvdcss->i_read_fd = dvdcss->i_raw_fd;
    }

    return 0;
}
#endif

int _dvdcss_close ( dvdcss_t dvdcss )
{
#if defined( WIN32 )
    if( dvdcss->b_file )
    {
        close( dvdcss->i_fd );
    }
    else if( WIN2K )
    {
        CloseHandle( (HANDLE) dvdcss->i_fd );
    }
    else /* ASPI */
    {

#if !defined(_XBOX)
        struct w32_aspidev *fd = (struct w32_aspidev *) dvdcss->i_fd;

        /* Unload aspi and free w32_aspidev structure */
        FreeLibrary( (HMODULE) fd->hASPI );
        free( (void*) dvdcss->i_fd );

#endif
    }

    /* Free readv temporary buffer */
    if( dvdcss->p_readv_buffer )
    {
        free( dvdcss->p_readv_buffer );
        dvdcss->p_readv_buffer   = NULL;
        dvdcss->i_readv_buf_size = 0;
    }

    return 0;
#else
close( dvdcss->i_fd );

    if( dvdcss->i_raw_fd >= 0 )
    {
        close( dvdcss->i_raw_fd );
        dvdcss->i_raw_fd = -1;
    }

    return 0;
#endif
}

/* Following functions are local */

/*****************************************************************************
 * Open commands.
 *****************************************************************************/
static int libc_open ( dvdcss_t dvdcss, char const *psz_device )
{
#if !defined( WIN32 )
    dvdcss->i_fd = dvdcss->i_read_fd = open( psz_device, 0 );
#else
dvdcss->i_fd = dvdcss->i_read_fd = open( psz_device, O_BINARY );
#endif

    if( dvdcss->i_fd == -1 )
    {
        _dvdcss_error( dvdcss, "failed opening device" );
        return -1;
    }

    return 0;
}

#if defined( WIN32 )
static int win2k_open ( dvdcss_t dvdcss, char const *psz_device )
{

#if defined( _XBOX )

	ANSI_STRING filename;

	OBJECT_ATTRIBUTES attributes;

	IO_STATUS_BLOCK status;

	NTSTATUS error;

//	DWORD dummy;



	RtlInitAnsiString(&filename, psz_device);

	InitializeObjectAttributes(&attributes, &filename, OBJ_CASE_INSENSITIVE, NULL);



	if (!NT_SUCCESS(error = NtCreateFile((HANDLE*) &dvdcss->i_fd, GENERIC_READ |

		SYNCHRONIZE | FILE_READ_ATTRIBUTES, &attributes, &status, NULL, 0,

		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, FILE_OPEN,

		FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT)))

	{

		return -1;

	}



#else
    char psz_dvd[7];
    _snprintf( psz_dvd, 7, "\\\\.\\%c:", psz_device[0] );

    /* To work around an M$ bug in IOCTL_DVD_READ_STRUCTURE, we need read
     * _and_ write access to the device (so we can make SCSI Pass Through
     * Requests). Unfortunately this is only allowed if you have
     * administrator priviledges so we allow for a fallback method with
     * only read access to the device (in this case ioctl_ReadCopyright()
     * won't send back the right result).
     * (See Microsoft Q241374: Read and Write Access Required for SCSI
     * Pass Through Requests) */
    (HANDLE) dvdcss->i_fd =
        CreateFile( psz_dvd, GENERIC_READ | GENERIC_WRITE,
                    FILE_SHARE_READ | FILE_SHARE_WRITE,
                    NULL, OPEN_EXISTING,
                    FILE_FLAG_RANDOM_ACCESS, NULL );

    if( (HANDLE) dvdcss->i_fd == INVALID_HANDLE_VALUE )
        (HANDLE) dvdcss->i_fd =
            CreateFile( psz_dvd, GENERIC_READ, FILE_SHARE_READ,
                        NULL, OPEN_EXISTING,
                        FILE_FLAG_RANDOM_ACCESS, NULL );

    if( (HANDLE) dvdcss->i_fd == INVALID_HANDLE_VALUE )
    {
        _dvdcss_error( dvdcss, "failed opening device" );
        return -1;
    }

#endif

    return 0;
}

static int aspi_open( dvdcss_t dvdcss, char const * psz_device )
{
#if !defined(_XBOX)

    HMODULE hASPI;
    DWORD dwSupportInfo;
    struct w32_aspidev *fd;
    int i, j, i_hostadapters;
    long (*lpGetSupport)( void );
    long (*lpSendCommand)( void* );
    char c_drive = psz_device[0];

    /* load aspi and init w32_aspidev structure */
    hASPI = LoadLibrary( "wnaspi32.dll" );
    if( hASPI == NULL )
    {
        _dvdcss_error( dvdcss, "unable to load wnaspi32.dll" );
        return -1;
    }

    (FARPROC) lpGetSupport = GetProcAddress( hASPI, "GetASPI32SupportInfo" );
    (FARPROC) lpSendCommand = GetProcAddress( hASPI, "SendASPI32Command" );

    if(lpGetSupport == NULL || lpSendCommand == NULL )
    {
        _dvdcss_error( dvdcss, "unable to get aspi function pointers" );
        FreeLibrary( hASPI );
        return -1;
    }

    dwSupportInfo = lpGetSupport();

    if( HIBYTE( LOWORD ( dwSupportInfo ) ) == SS_NO_ADAPTERS )
    {
        _dvdcss_error( dvdcss, "no ASPI adapters found" );
        FreeLibrary( hASPI );
        return -1;
    }

    if( HIBYTE( LOWORD ( dwSupportInfo ) ) != SS_COMP )
    {
        _dvdcss_error( dvdcss, "unable to initalize aspi layer" );
        FreeLibrary( hASPI );
        return -1;
    }

    i_hostadapters = LOBYTE( LOWORD( dwSupportInfo ) );
    if( i_hostadapters == 0 )
    {
        _dvdcss_error( dvdcss, "no ASPI adapters ready" );
        FreeLibrary( hASPI );
        return -1;
    }

    fd = malloc( sizeof( struct w32_aspidev ) );
    if( fd == NULL )
    {
        _dvdcss_error( dvdcss, "not enough memory" );
        FreeLibrary( hASPI );
        return -1;
    }

    fd->i_blocks = 0;
    fd->hASPI = (long) hASPI;
    fd->lpSendCommand = lpSendCommand;

    c_drive = c_drive > 'Z' ? c_drive - 'a' : c_drive - 'A';

    for( i = 0; i < i_hostadapters; i++ )
    {
        for( j = 0; j < 15; j++ )
        {
            struct SRB_GetDiskInfo srbDiskInfo;

            srbDiskInfo.SRB_Cmd         = SC_GET_DISK_INFO;
            srbDiskInfo.SRB_HaId        = i;
            srbDiskInfo.SRB_Flags       = 0;
            srbDiskInfo.SRB_Hdr_Rsvd    = 0;
            srbDiskInfo.SRB_Target      = j;
            srbDiskInfo.SRB_Lun         = 0;

            lpSendCommand( (void*) &srbDiskInfo );

            if( (srbDiskInfo.SRB_Status == SS_COMP) &&
                    (srbDiskInfo.SRB_Int13HDriveInfo == c_drive) )
            {
                /* Make sure this is a cdrom device */
                struct SRB_GDEVBlock srbGDEVBlock;

                memset( &srbGDEVBlock, 0, sizeof(struct SRB_GDEVBlock) );
                srbGDEVBlock.SRB_Cmd    = SC_GET_DEV_TYPE;
                srbGDEVBlock.SRB_HaId   = i;
                srbGDEVBlock.SRB_Target = j;

                lpSendCommand( (void*) &srbGDEVBlock );

                if( ( srbGDEVBlock.SRB_Status == SS_COMP ) &&
                        ( srbGDEVBlock.SRB_DeviceType == DTYPE_CDROM ) )
                {
                    fd->i_sid = MAKEWORD( i, j );
                    dvdcss->i_fd = (int) fd;
                    return 0;
                }
                else
                {
                    free( (void*) fd );
                    FreeLibrary( hASPI );
                    _dvdcss_error( dvdcss,"this is not a cdrom drive" );
                    return -1;
                }
            }
        }
    }

    free( (void*) fd );
    FreeLibrary( hASPI );
    _dvdcss_error( dvdcss, "unable to get haid and target (aspi)" );
#endif
    return -1;
}
#endif

/*****************************************************************************
 * Seek commands.
 *****************************************************************************/
static int libc_seek( dvdcss_t dvdcss, int i_blocks )
{
    off_t   i_read;

    dvdcss->i_seekpos = i_blocks;

    i_read = lseek( dvdcss->i_read_fd,
                    (off_t)i_blocks * (off_t)DVDCSS_BLOCK_SIZE, SEEK_SET );

    if( i_read < 0 )
    {
        _dvdcss_error( dvdcss, "seek error" );
    }

    return i_read / DVDCSS_BLOCK_SIZE;
}

#if defined( WIN32 )
static int win2k_seek( dvdcss_t dvdcss, int i_blocks )
{
    LARGE_INTEGER li_read;

    dvdcss->i_seekpos = i_blocks;

#ifndef INVALID_SET_FILE_POINTER
#   define INVALID_SET_FILE_POINTER ((DWORD)-1)
#endif

    li_read.QuadPart = (LONGLONG)i_blocks * DVDCSS_BLOCK_SIZE;

    li_read.LowPart = SetFilePointer( (HANDLE) dvdcss->i_fd,
                                      li_read.LowPart,
                                      &li_read.HighPart, FILE_BEGIN );
    if( (li_read.LowPart == INVALID_SET_FILE_POINTER)
            && GetLastError() != NO_ERROR)
    {
        li_read.QuadPart = -DVDCSS_BLOCK_SIZE;
    }

    li_read.QuadPart /= DVDCSS_BLOCK_SIZE;
    return (int)li_read.QuadPart;
}

static int aspi_seek( dvdcss_t dvdcss, int i_blocks )
{
    int i_old_blocks;
    char sz_buf[ DVDCSS_BLOCK_SIZE ];
    struct w32_aspidev *fd = (struct w32_aspidev *) dvdcss->i_fd;

    i_old_blocks = fd->i_blocks;
    fd->i_blocks = i_blocks;

    if( aspi_read_internal( dvdcss->i_fd, sz_buf, 1 ) == -1 )
    {
        fd->i_blocks = i_old_blocks;
        return -1;
    }

    (fd->i_blocks)--;

    return fd->i_blocks;
}
#endif

/*****************************************************************************
 * Read commands.
 *****************************************************************************/
static int libc_read ( dvdcss_t dvdcss, void *p_buffer, int i_blocks )
{
    int i_ret;
    /* TODO: partial reads are wrong,i.e 2200/2048 = 1
     * but the location has advanced 2200 bytes (lseek possition that is) */
    i_ret = read( dvdcss->i_read_fd, p_buffer,
                  (off_t)i_blocks * DVDCSS_BLOCK_SIZE );
    if( i_ret < 0 )
    {
        _dvdcss_error( dvdcss, "read error" );
        return i_ret;
    }

    return i_ret / DVDCSS_BLOCK_SIZE;
}

#if defined( WIN32 )
static int win2k_read ( dvdcss_t dvdcss, void *p_buffer, int i_blocks )
{
    int i_bytes;

    if( !ReadFile( (HANDLE) dvdcss->i_fd, p_buffer,
                   i_blocks * DVDCSS_BLOCK_SIZE,
                   (LPDWORD)&i_bytes, NULL ) )
    {
        return -1;
    }

    return i_bytes / DVDCSS_BLOCK_SIZE;
}

static int aspi_read ( dvdcss_t dvdcss, void *p_buffer, int i_blocks )
{
    return aspi_read_internal( dvdcss->i_fd, p_buffer, i_blocks );
}
#endif

/*****************************************************************************
 * Readv commands.
 *****************************************************************************/
static int libc_readv ( dvdcss_t dvdcss, struct iovec *p_iovec, int i_blocks )
{
#if defined( WIN32 )
    int i_index, i_len, i_total = 0;
    unsigned char *p_base;
    int i_bytes;

    for( i_index = i_blocks;
            i_index;
            i_index--, p_iovec++ )
    {
        i_len  = p_iovec->iov_len;
        p_base = p_iovec->iov_base;

        if( i_len <= 0 )
        {
            continue;
        }

        i_bytes = read( dvdcss->i_fd, p_base, i_len );

        if( i_bytes < 0 )
        {
            /* One of the reads failed, too bad.
             * We won't even bother returning the reads that went ok,
             * and as in the posix spec the file postition is left
             * unspecified after a failure */
            return -1;
        }

        i_total += i_bytes;

        if( i_bytes != i_len )
        {
            /* We reached the end of the file or a signal interrupted
             * the read. Return a partial read. */
            return i_total / DVDCSS_BLOCK_SIZE;
        }
    }

    return i_total / DVDCSS_BLOCK_SIZE;
#else
int i_read = readv( dvdcss->i_read_fd, p_iovec, i_blocks );

    return i_read / DVDCSS_BLOCK_SIZE;
#endif
}

#if defined( WIN32 )
/*****************************************************************************
 * win_readv: vectored read using ReadFile for Win2K and ASPI for win9x
 *****************************************************************************/
static int win_readv ( dvdcss_t dvdcss, struct iovec *p_iovec, int i_blocks )
{
    int i_index;
    int i_blocks_read, i_blocks_total = 0;

    /* Check the size of the readv temp buffer, just in case we need to
     * realloc something bigger */
    if( dvdcss->i_readv_buf_size < i_blocks * DVDCSS_BLOCK_SIZE )
    {
        dvdcss->i_readv_buf_size = i_blocks * DVDCSS_BLOCK_SIZE;

        if( dvdcss->p_readv_buffer )
            free( dvdcss->p_readv_buffer );

        /* Allocate a buffer which will be used as a temporary storage
         * for readv */
        dvdcss->p_readv_buffer = malloc( dvdcss->i_readv_buf_size );
        if( !dvdcss->p_readv_buffer )
        {
            _dvdcss_error( dvdcss, " failed (readv)" );
            return -1;
        }
    }

    for( i_index = i_blocks; i_index; i_index-- )
    {
        i_blocks_total += p_iovec[i_index-1].iov_len;
    }

    if( i_blocks_total <= 0 )
        return 0;

    i_blocks_total /= DVDCSS_BLOCK_SIZE;

    if( WIN2K )
    {
        unsigned long int i_bytes;
        if( !ReadFile( (HANDLE)dvdcss->i_fd, dvdcss->p_readv_buffer,
                       i_blocks_total * DVDCSS_BLOCK_SIZE, &i_bytes, NULL ) )
        {
            /* The read failed... too bad.
             * As in the posix spec the file postition is left
             * unspecified after a failure */
            return -1;
        }
        i_blocks_read = i_bytes / DVDCSS_BLOCK_SIZE;
    }
    else /* Win9x */
    {
        i_blocks_read = aspi_read_internal( dvdcss->i_fd,
                                            dvdcss->p_readv_buffer,
                                            i_blocks_total );
        if( i_blocks_read < 0 )
        {
            /* See above */
            return -1;
        }
    }

    /* We just have to copy the content of the temp buffer into the iovecs */
    for( i_index = 0, i_blocks_total = i_blocks_read;
            i_blocks_total > 0;
            i_index++ )
    {
        memcpy( p_iovec[i_index].iov_base,
                dvdcss->p_readv_buffer + (i_blocks_read - i_blocks_total)
                * DVDCSS_BLOCK_SIZE,
                p_iovec[i_index].iov_len );
        /* if we read less blocks than asked, we'll just end up copying
         * garbage, this isn't an issue as we return the number of
         * blocks actually read */
        i_blocks_total -= ( p_iovec[i_index].iov_len / DVDCSS_BLOCK_SIZE );
    }

    return i_blocks_read;
}

static int aspi_read_internal( int i_fd, void *p_data, int i_blocks )
{
    HANDLE hEvent;
    struct SRB_ExecSCSICmd ssc;
    struct w32_aspidev *fd = (struct w32_aspidev *) i_fd;

    /* Create the transfer completion event */
    hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
    if( hEvent == NULL )
    {
        return -1;
    }

    memset( &ssc, 0, sizeof( ssc ) );

    ssc.SRB_Cmd         = SC_EXEC_SCSI_CMD;
    ssc.SRB_Flags       = SRB_DIR_IN | SRB_EVENT_NOTIFY;
    ssc.SRB_HaId        = LOBYTE( fd->i_sid );
    ssc.SRB_Target      = HIBYTE( fd->i_sid );
    ssc.SRB_SenseLen    = SENSE_LEN;

    ssc.SRB_PostProc = (LPVOID) hEvent;
    ssc.SRB_BufPointer  = p_data;
    ssc.SRB_CDBLen      = 12;

    ssc.CDBByte[0]      = 0xA8; /* RAW */
    ssc.CDBByte[2]      = (UCHAR) (fd->i_blocks >> 24);
    ssc.CDBByte[3]      = (UCHAR) (fd->i_blocks >> 16) & 0xff;
    ssc.CDBByte[4]      = (UCHAR) (fd->i_blocks >> 8) & 0xff;
    ssc.CDBByte[5]      = (UCHAR) (fd->i_blocks) & 0xff;

    /* We have to break down the reads into 64kb pieces (ASPI restriction) */
    if( i_blocks > 32 )
    {
        ssc.SRB_BufLen = 32 * DVDCSS_BLOCK_SIZE;
        ssc.CDBByte[9] = 32;
        fd->i_blocks  += 32;

        /* Initiate transfer */
        ResetEvent( hEvent );
        fd->lpSendCommand( (void*) &ssc );

        /* transfer the next 64kb (aspi_read_internal is called recursively)
         * We need to check the status of the read on return */
        if( aspi_read_internal( i_fd, (u8*) p_data + 32 * DVDCSS_BLOCK_SIZE,
                                i_blocks - 32) < 0 )
        {
            return -1;
        }
    }
    else
    {
        /* This is the last transfer */
        ssc.SRB_BufLen   = i_blocks * DVDCSS_BLOCK_SIZE;
        ssc.CDBByte[9]   = (UCHAR) i_blocks;
        fd->i_blocks += i_blocks;

        /* Initiate transfer */
        ResetEvent( hEvent );
        fd->lpSendCommand( (void*) &ssc );

    }

    /* If the command has still not been processed, wait until it's finished */
    if( ssc.SRB_Status == SS_PENDING )
    {
        WaitForSingleObject( hEvent, INFINITE );
    }
    CloseHandle( hEvent );

    /* check that the transfer went as planned */
    if( ssc.SRB_Status != SS_COMP )
    {
        return -1;
    }

    return i_blocks;
}
#endif

