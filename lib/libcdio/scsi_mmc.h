/*
    $Id: scsi_mmc.h,v 1.1 2004/11/08 18:45:24 wiso Exp $

    Copyright (C) 2003, 2004 Rocky Bernstein <rocky@panix.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*!
   \file scsi_mmc.h 
   \brief Common definitions for SCSI MMC (Multi-Media Commands).
*/

#ifndef __SCSI_MMC_H__
#define __SCSI_MMC_H__

#include <cdio/cdio.h>
#include <cdio/types.h>
#include <cdio/dvd.h>

/*! The generic packet command opcodes for CD/DVD Logical Units. */

#define CDIO_MMC_GPCMD_INQUIRY 	             0x12
#define CDIO_MMC_GPCMD_MODE_SELECT_6	     0x15
#define CDIO_MMC_GPCMD_MODE_SENSE 	     0x1a
#define CDIO_MMC_GPCMD_START_STOP            0x1b
#define CDIO_MMC_GPCMD_ALLOW_MEDIUM_REMOVAL  0x1e
#define CDIO_MMC_GPCMD_READ_10	             0x28

/*!
  	Group 2 Commands
  */
#define CDIO_MMC_GPCMD_READ_SUBCHANNEL	     0x42
#define CDIO_MMC_GPCMD_READ_TOC              0x43
#define CDIO_MMC_GPCMD_READ_HEADER           0x44
#define CDIO_MMC_GPCMD_PLAY_AUDIO_10         0x45
#define CDIO_MMC_GPCMD_GET_CONFIGURATION     0x46
#define CDIO_MMC_GPCMD_PLAY_AUDIO_MSF        0x47
#define CDIO_MMC_GPCMD_PLAY_AUDIO_TI         0x48
#define CDIO_MMC_GPCMD_PLAY_TRACK_REL_10     0x49
#define CDIO_MMC_GPCMD_PAUSE_RESUME          0x4b

#define CDIO_MMC_GPCMD_READ_DISC_INFO	     0x51
#define CDIO_MMC_GPCMD_MODE_SELECT	     0x55
#define CDIO_MMC_GPCMD_MODE_SENSE_10	     0x5a

/*!
 	Group 5 Commands
 */
#define CDIO_MMC_GPCMD_PLAY_AUDIO_12	     0xa5
#define CDIO_MMC_GPCMD_READ_12	             0xa8
#define CDIO_MMC_GPCMD_PLAY_TRACK_REL_12     0xa9
#define CDIO_MMC_GPCMD_READ_DVD_STRUCTURE    0xad
#define CDIO_MMC_GPCMD_READ_CD	             0xbe
#define CDIO_MMC_GPCMD_READ_MSF	             0xb9

/*!
 	Group 6 Commands
 */

#define	CDIO_MMC_GPCMD_CD_PLAYBACK_STATUS    0xc4 /**< SONY unique command */
#define	CDIO_MMC_GPCMD_PLAYBACK_CONTROL      0xc9 /**< SONY unique command */
#define	CDIO_MMC_GPCMD_READ_CDDA	     0xd8 /**< Vendor unique command */
#define	CDIO_MMC_GPCMD_READ_CDXA	     0xdb /**< Vendor unique command */
#define	CDIO_MMC_GPCMD_READ_ALL_SUBCODES     0xdf /**< Vendor unique command */



/*! Level values that can go into READ_CD */
#define CDIO_MMC_READ_TYPE_ANY   0  /**< All types */
#define CDIO_MMC_READ_TYPE_CDDA  1  /**< Only CD-DA sectors */
#define CDIO_MMC_READ_TYPE_MODE1 2  /**< mode1 sectors (user data = 2048) */
#define CDIO_MMC_READ_TYPE_MODE2 3  /**< mode2 sectors form1 or form2 */
#define CDIO_MMC_READ_TYPE_M2F1  4  /**< mode2 sectors form1 */
#define CDIO_MMC_READ_TYPE_M2F2  5  /**< mode2 sectors form2 */

/*! Format values for READ_TOC */
#define CDIO_MMC_READTOC_FMT_TOC      0
#define CDIO_MMC_READTOC_FMT_SESSION  1  
#define CDIO_MMC_READTOC_FMT_FULTOC   2  
#define CDIO_MMC_READTOC_FMT_PMA      3  /**< Q subcode data */
#define CDIO_MMC_READTOC_FMT_ATIP     4  /**< includes media type */
#define CDIO_MMC_READTOC_FMT_CDTEXT   5  /**< CD-TEXT info  */

/*! Page codes for MODE SENSE and MODE SET. */
#define CDIO_MMC_R_W_ERROR_PAGE		0x01
#define CDIO_MMC_WRITE_PARMS_PAGE	0x05
#define CDIO_MMC_AUDIO_CTL_PAGE		0x0e
#define CDIO_MMC_CDR_PARMS_PAGE		0x0d
#define CDIO_MMC_POWER_PAGE		0x1a
#define CDIO_MMC_FAULT_FAIL_PAGE	0x1c
#define CDIO_MMC_TO_PROTECT_PAGE	0x1d
#define CDIO_MMC_CAPABILITIES_PAGE	0x2a
#define CDIO_MMC_ALL_PAGES		0x3f

/*! Return type codes for GET_CONFIGURATION. */
#define CDIO_MMC_GET_CONF_ALL_FEATURES     0  /**< all features without regard
					           to currency. */
#define CDIO_MMC_GET_CONF_CURRENT_FEATURES 1  /**< features which are currently
					           in effect (e.g. based on
					           medium inserted). */
#define CDIO_MMC_GET_CONF_NAMED_FEATURE    2  /**< just the feature named in
					           the GET_CONFIGURATION 
					           cdb. */

/*! FEATURE codes used in GET CONFIGURATION. */

#define CDIO_MMC_FEATURE_PROFILE_LIST     0x000 /**< Profile List Feature */
#define CDIO_MMC_FEATURE_CORE             0x001 
#define CDIO_MMC_FEATURE_REMOVABLE_MEDIUM 0x002 /**< Removable Medium
						   Feature */
#define CDIO_MMC_FEATURE_WRITE_PROTECT    0x003 /**< Write Protect
						   Feature */
#define CDIO_MMC_FEATURE_RANDOM_READABLE  0x010 /**< Random Readable
						   Feature */
#define CDIO_MMC_FEATURE_MULTI_READ       0x01D /**< Multi-Read
						   Feature */
#define CDIO_MMC_FEATURE_CD_READ          0x01E /**< CD Read
						   Feature */
#define CDIO_MMC_FEATURE_DVD_READ         0x01F /**< DVD Read
						   Feature */
#define CDIO_MMC_FEATURE_RANDOM_WRITABLE  0x020 /**< Random Writable
						   Feature */
#define CDIO_MMC_FEATURE_INCR_WRITE       0x021 /**< Incremental
						   Streaming Writable
						   Feature */
#define CDIO_MMC_FEATURE_SECTOR_ERASE     0x022 /**< Sector Erasable
						   Feature */
#define CDIO_MMC_FEATURE_FORMATABLE       0x023 /**< Formattable
						   Feature */
#define CDIO_MMC_FEATURE_DEFECT_MGMT      0x024 /**< Management
						   Ability of the
						   Logical Unit/media
						   system to provide
						   an apparently
						   defect-free
						   space.*/
#define CDIO_MMC_FEATURE_WRITE_ONCE       0x025 /**< Write Once
						   Feature */
#define CDIO_MMC_FEATURE_RESTRICT_OVERW   0x026 /**< Restricted
						   Overwrite
						   Feature */
#define CDIO_MMC_FEATURE_CD_RW_CAV        0x027 /**< CD-RW CAV Write
						   Feature */
#define CDIO_MMC_FEATURE_MRW              0x028 /**< MRW Feature */
#define CDIO_MMC_FEATURE_DVD_PRW          0x02A /**< DVD+RW Feature */
#define CDIO_MMC_FEATURE_DVD_PR           0x02B /**< DVD+R Feature */
#define CDIO_MMC_FEATURE_CD_TAO           0x02D
#define CDIO_MMC_FEATURE_CD_SAO           0x02E
#define CDIO_MMC_FEATURE_POWER_MGMT       0x100 /**< Initiator and
						   device directed
						   power management */
#define CDIO_MMC_FEATURE_CDDA_EXT_PLAY    0x103 /**< Ability to play
						   audio CDs via the
						   Logical Unit s own
						   analog output */
#define CDIO_MMC_FEATURE_MCODE_UPGRADE    0x104 /* Ability for the
						   device to accept
						   new microcode via
						   the interface */
#define CDIO_MMC_FEATURE_TIME_OUT         0x105 /**< Ability to
						   respond to all
						   commands within a
						   specific time */
#define CDIO_MMC_FEATURE_DVD_CSS          0x106 /**< Ability to
						   perform DVD
						   CSS/CPPM
						   authentication and
						   RPC */
#define CDIO_MMC_FEATURE_RT_STREAMING     0x107 /**< Ability to read
						   and write using
						   Initiator requested
						   performance
						   parameters
						   */
#define CDIO_MMC_FEATURE_LU_SN            0x108 /**< The Logical Unit
						   has a unique
						   identifier. */
#define CDIO_MMC_FEATURE_FIRMWARE_DATE    0x1FF /**< Firmware creation
						   date report */
				
/*! Profile codes used in GET_CONFIGURATION - PROFILE LIST. */
#define CDIO_MMC_FEATURE_PROF_NON_REMOVABLE 0x0001 /**< Re-writable
						     disk, capable of
						     changing
						     behavior */
#define CDIO_MMC_FEATURE_PROF_REMOVABLE     0x0002 /**< disk
						      Re-writable;
						      with removable
						      media */
#define CDIO_MMC_FEATURE_PROF_MO_ERASABLE   0x0003 /**< Erasable
						      Magneto-Optical
						      disk with sector
						      erase
						      capability */
#define CDIO_MMC_FEATURE_PROF_MO_WRITE_ONCE 0x0004 /**< Write Once
						      Magneto-Optical
						      write once */
#define CDIO_MMC_FEATURE_PROF_AS_MO         0x0005 /**< Advance
						      Storage
						      Magneto-Optical */
#define CDIO_MMC_FEATURE_PROF_CD_ROM        0x0008 /**< Read only
						      Compact Disc
						      capable */
#define CDIO_MMC_FEATURE_PROF_CD_R          0x0009 /**< Write once
						      Compact Disc
						      capable */
#define CDIO_MMC_FEATURE_PROF_CD_RW         0x000A /**< CD-RW
						      Re-writable
						      Compact Disc
						      capable */
#define CDIO_MMC_FEATURE_PROF_DVD_ROM       0x0010 /**< Read only
						      DVD */
#define CDIO_MMC_FEATURE_PROF_DVD_R_SEQ     0x0011 /**< Re-recordable
						      DVD using
						      Sequential
						      recording */
#define CDIO_MMC_FEATURE_PROF_DVD_RAM       0x0012 /**< Re-writable
						      DVD */
#define CDIO_MMC_FEATURE_PROF_DVD_RW_RO     0x0013 /**< Re-recordable
						      DVD using
						      Restricted
						      Overwrite */
#define CDIO_MMC_FEATURE_PROF_DVD_RW_SEQ    0x0014 /**< Re-recordable
						      DVD using
						      Sequential
						      recording */
#define CDIO_MMC_FEATURE_PROF_DVD_PRW       0x001A /**< DVD+RW - DVD
						      ReWritable */
#define CDIO_MMC_FEATURE_PROF_DVD_PR        0x001B /**< DVD+R - DVD
						      Recordable */
#define CDIO_MMC_FEATURE_PROF_DDCD_ROM      0x0020 /**< Read only
						      DDCD */
#define CDIO_MMC_FEATURE_PROF_DDCD_R        0x0021 /**< DDCD-R Write
						      only DDCD */
#define CDIO_MMC_FEATURE_PROF_DDCD_RW       0x0022 /**< Re-Write only
						      DDCD */
#define CDIO_MMC_FEATURE_PROF_NON_CONFORM   0xFFFF /**< The Logical
						      Unit does not
						      conform to any
						      Profile. */

/*! This is listed as optional in ATAPI 2.6, but is (curiously) 
  missing from Mt. Fuji, Table 57.  It _is_ mentioned in Mt. Fuji
  Table 377 as an MMC command for SCSi devices though...  Most ATAPI
  drives support it. */
#define CDIO_MMC_GPCMD_SET_SPEED	0xbb


/*! The largest Command Descriptor Buffer (CDB) size.
    The possible sizes are 6, 10, and 12 bytes.
 */
#define MAX_CDB_LEN 12

/*! A Command Descriptor Buffer (CDB) used in sending SCSI MMC 
    commands.
 */
typedef struct scsi_mmc_cdb {
  uint8_t field[MAX_CDB_LEN];
} scsi_mmc_cdb_t;

typedef struct scsi_mmc_feature_list_header {
  unsigned char length_msb;
  unsigned char length_1sb;
  unsigned char length_2sb;
  unsigned char length_lsb;
  unsigned char reserved1;
  unsigned char reserved2;
  unsigned char profile_msb;
  unsigned char profile_lsb;
} scs_mmc_feature_list_header_t;

/*! An enumeration indicating whether a SCSI MMC command is sending
    data or getting data.
 */
typedef enum scsi_mmc_direction {
  SCSI_MMC_DATA_READ,
  SCSI_MMC_DATA_WRITE
} scsi_mmc_direction_t;

#define CDIO_MMC_SET_COMMAND(cdb, command) \
  cdb[0] = command

#define CDIO_MMC_SET_READ_TYPE(cdb, sector_type) \
  cdb[1] = (sector_type << 2)

#define CDIO_MMC_GET_LEN16(p) \
  (p[0]<<8) + p[1]

#define CDIO_MMC_GET_LEN32(p) \
  (p[0] << 24) + (p[1] << 16) + (p[2] << 8) + p[3];

#define CDIO_MMC_SET_LEN16(cdb, pos, len)  \
  cdb[pos  ] = (len >>  8) & 0xff; \
  cdb[pos+1] = (len      ) & 0xff

#define CDIO_MMC_SET_READ_LBA(cdb, lba) \
  cdb[2] = (lba >> 24) & 0xff; \
  cdb[3] = (lba >> 16) & 0xff; \
  cdb[4] = (lba >>  8) & 0xff; \
  cdb[5] = (lba      ) & 0xff

#define CDIO_MMC_SET_START_TRACK(cdb, command) \
  cdb[6] = command

#define CDIO_MMC_SET_READ_LENGTH24(cdb, len) \
  cdb[6] = (len >> 16) & 0xff; \
  cdb[7] = (len >>  8) & 0xff; \
  cdb[8] = (len      ) & 0xff

#define CDIO_MMC_SET_READ_LENGTH16(cdb, len) \
  CDIO_MMC_SET_LEN16(cdb, 7, len)

#define CDIO_MMC_SET_READ_LENGTH8(cdb, len) \
  cdb[8] = (len      ) & 0xff

#define CDIO_MMC_MCSB_ALL_HEADERS 0x78

#define CDIO_MMC_SET_MAIN_CHANNEL_SELECTION_BITS(cdb, val) \
  cdb[9] = val;

/*!  
  Return the number of length in bytes of the Command Descriptor
  buffer (CDB) for a given SCSI MMC command. The length will be 
  either 6, 10, or 12. 
*/
uint8_t scsi_mmc_get_cmd_len(uint8_t scsi_cmd);


/*!
  Run a SCSI MMC command. 
 
  cdio	        CD structure set by cdio_open().
  i_timeout_ms  time in milliseconds we will wait for the command
                to complete. 
  p_cdb	        CDB bytes. All values that are needed should be set on 
                input. We'll figure out what the right CDB length should be.
  e_direction	direction the transfer is to go.
  i_buf	        Size of buffer
  p_buf	        Buffer for data, both sending and receiving.

  Returns 0 if command completed successfully.
 */
int scsi_mmc_run_cmd( const CdIo *p_cdio, unsigned int i_timeout_ms,
		      const scsi_mmc_cdb_t *p_cdb,
		      scsi_mmc_direction_t e_direction, unsigned int i_buf, 
		      /*in/out*/ void *p_buf );

/*!
 * Eject using SCSI MMC commands. Return 0 if successful.
 */
int scsi_mmc_eject_media( const CdIo *p_cdio);

/*! Packet driver to read mode2 sectors. 
   Can read only up to 25 blocks.
*/
int scsi_mmc_read_sectors ( const CdIo *p_cdio, void *p_buf, lba_t lba, 
			    int sector_type, unsigned int nblocks);

/*!
  Set the block size for subsequest read requests, via a SCSI MMC 
  MODE_SELECT 6 command.
 */
int scsi_mmc_set_blocksize ( const CdIo *p_cdio, unsigned int bsize);

/*!
  Return the the kind of drive capabilities of device.
 */
void scsi_mmc_get_drive_cap (const CdIo *p_cdio,
			     /*out*/ cdio_drive_read_cap_t  *p_read_cap,
			     /*out*/ cdio_drive_write_cap_t *p_write_cap,
			     /*out*/ cdio_drive_misc_cap_t  *p_misc_cap);

/*! 
  Get the DVD type associated with cd object.
*/
discmode_t scsi_mmc_get_dvd_struct_physical ( const CdIo *p_cdio, 
					      cdio_dvd_struct_t *s);

/*! 
  Get the CD-ROM hardware info via a SCSI MMC INQUIRY command.
  False is returned if we had an error getting the information.
*/
bool scsi_mmc_get_hwinfo ( const CdIo *p_cdio, 
			   /* out*/ cdio_hwinfo_t *p_hw_info );


/*!
  Get the media catalog number (MCN) from the CD via MMC.
  
  @return the media catalog number r NULL if there is none or we
  don't have the ability to get it.
  
  Note: string is malloc'd so caller has to free() the returned
  string when done with it.
  
*/
char *scsi_mmc_get_mcn ( const CdIo *p_cdio );

#endif /* __SCSI_MMC_H__ */
