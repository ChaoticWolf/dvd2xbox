#include "d2xutils.h"

#include <helper.h>



D2Xutils::D2Xutils()
{

}

D2Xutils::~D2Xutils()
{

}

ULONG D2Xutils::getTitleID(char* path)
{
	FILE *stream;
	_XBE_CERTIFICATE HC;
	_XBE_HEADER HS;

	stream  = fopen( path, "rb" );
	if(stream != NULL) {
		fread(&HS,1,sizeof(HS),stream);
		fseek(stream,HS.XbeHeaderSize,SEEK_SET);
		fread(&HC,1,sizeof(HC),stream);
		fclose(stream);
		return HC.TitleId;
	}	
	return 0;
}


bool D2Xutils::writeTitleName(char* path,const WCHAR* title)
{
	FILE *stream;
	_XBE_CERTIFICATE HC; 
	_XBE_HEADER HS;

	stream  = fopen( path, "r+b" );
	if(stream != NULL) {
		fseek(stream,0,SEEK_SET);
		fread(&HS,1,sizeof(HS),stream);
		fseek(stream,HS.XbeHeaderSize,SEEK_SET);
		fread(&HC,sizeof(HC),1,stream);
		wcsncpy(HC.TitleName,title,40);
		//HC.GameRegion = 0x80000007;
		//HC.MediaTypes = 0x000002FF;
		fseek(stream,HS.XbeHeaderSize,SEEK_SET);
		fwrite(&HC,sizeof(HC),1,stream);
		fclose(stream);
		return 1;
	}	
	return 0;
}

int D2Xutils::char2byte(char* ch, BYTE* b)
{
	char tmp[100];
	if(strlen(ch)%2)
		return 1;
	int n_byte = strlen(ch)/2;
	for(int c=0;c<n_byte;c++)
	{
		sprintf(tmp,"%c%c",ch[2*c],ch[2*c+1]);
		b[c] = (BYTE)strtoul(tmp,'\0',16);
	}
	return 0;
}

void D2Xutils::str2byte(string item,BYTE* buf)
{
	char buffer[4];
	for(int i=0;i<item.size();i++)
	{
		sprintf(buffer,"%2X",item.substr(i,1).c_str());
		buf[i] = (BYTE)strtoul(buffer,'\0',16);
	}
}

void D2Xutils::str2hex(string item,char* buf)
{
	char buffer[4];
	for(int i=0;i<item.size();i++)
	{
		sprintf(buffer,"%2X",item.substr(i,1).c_str());
		buf[i] = buffer[0];
		buf[i+1] = buffer[1];
		buf[i+2] = '\0';
	}
}


int D2Xutils::findHex(const char* file,char* mtext,int offset)
{
	FILE *stream;
	int read;
	#define blocksize 600
	bool found = false;
	BYTE tmp[blocksize];
	BYTE btext[100];
	if(strlen(mtext)%2)
		return -1;
	int n_byte = strlen(mtext)/2;
	
	if(char2byte(mtext,btext))
		return -1;

	//DPf_H("Checking %s",mtext);

	stream  = fopen( file, "rb" );
	if(stream==NULL)
		return -2;
	while(!found)
	{
		fseek(stream,offset,SEEK_SET);
		read = fread(tmp,1,blocksize,stream);
		if(read<n_byte) break;
		
		int c2=0;
		for(int c=0;c<read;c++)
		{
			if(btext[c2] != tmp[c])
			{
				c2=0;
				continue;
			}
			if(c2 == n_byte-1)
			{
				offset+=c-n_byte+1;
				found = true;
				break;
			}
			c2++;
		}
		if(!found) offset+=(blocksize-n_byte);
	}
	fclose(stream);
	if(found)
		return offset;
	return -1;
}

int D2Xutils::writeHex(const char* file,char* mtext,int offset)
{
	FILE *stream;
	BYTE btext[100];
	int write;

	if(strlen(mtext)%2)
		return -1;
	int n_byte = strlen(mtext)/2;

	if(char2byte(mtext,btext))
		return -1;

	SetFileAttributes(file,FILE_ATTRIBUTE_NORMAL);
	stream  = fopen( file, "r+b" );
	if(stream==NULL)
		return -2;
	fseek(stream,offset,SEEK_SET);
	write = fwrite(btext,1,n_byte,stream);
	if(write != n_byte)
		return -3;

	fclose(stream);
	return 0;

}


void D2Xutils::addSlash(char* source)
{
	if(source[strlen(source)-1] != '\\')
		strcat(source,"\\");
}

bool D2Xutils::DelTree(char *path)
{
	char* sourcesearch = new char[1024];
	char* sourcefile = new char[1024];
	WIN32_FIND_DATA wfd;
	HANDLE hFind;

	strcpy(sourcesearch,path);
	strcat(sourcesearch,"\\*");

	// Start the find and check for failure.
	hFind = FindFirstFile( sourcesearch, &wfd );

	if( INVALID_HANDLE_VALUE == hFind )
	{
		RemoveDirectory( path );
	    return true;
	}
	else
	{
	    // Display each file and ask for the next.
	    do
	    {
			strcpy(sourcefile,path);
			strcat(sourcefile,"\\");
			strcat(sourcefile,wfd.cFileName);

			// Only do files
			if(wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
			{
				//strcat(sourcefile,"\\");
				// Recursion
				if(!DelTree( sourcefile ))
				{
					if(sourcesearch != NULL)
					{
						delete sourcesearch;
						sourcesearch = NULL;
					}
					if(sourcefile != NULL)
					{
						delete sourcefile;
						sourcefile = NULL;
					}
					return false;
				}
				DPf_H("Called deltree with: %hs",sourcefile);
				//if(!RemoveDirectory(sourcefile))
				//	return false;
				DPf_H("Called removedir with: %hs",sourcefile);
			
			}
			else
			{
				// delete file
				DPf_H("Called delfile with: %hs",sourcefile);
				if(!DeleteFile(sourcefile))
					return false;
	
			}

	    }
	    while(FindNextFile( hFind, &wfd ));

	    // Close the find handle.
	    FindClose( hFind );
	}
	RemoveDirectory( path );
	if(sourcesearch != NULL)
	{
		delete sourcesearch;
		sourcesearch = NULL;
	}
	if(sourcefile != NULL)
	{
		delete sourcefile;
		sourcefile = NULL;
	}
	return true;
}

void D2Xutils::getHomePath(char* path)
{
	CIoSupport p_IO;
	p_IO.GetXbePath(path);
	if(strlen(path) > 2)
	{
		char* p_xbe = strrchr(path,'\\');
		*p_xbe = 0;
	} else
	{
		strcpy(path,"e:\\");
	}
}

// MXM utils

HRESULT D2Xutils::MakePath( LPCTSTR szPath )
{
	HRESULT hr = E_FAIL;

	// Generate path from start to finish!
	CStdString sPath(szPath);

	sPath = PathSlasher( szPath, false );
#if 0
	int iPos = sPath.GetLength()-1;
	// Strip trailing backslash...
	if ( iPos >= 0 && sPath[iPos] == _T('\\') )
	{
		sPath = sPath.Left( iPos );
	}
#endif
	DWORD dwAttr = GetFileAttributes( sPath );

	if ( sPath.GetLength() > 2 )
	{
		if ( dwAttr == (-1))
		{
			// OK, let's step through the process of making a path
			if ( _istalpha( sPath[0] ) && sPath[1] == _T(':') && sPath[2] == _T('\\') )
			{
				// OK so far... let's see if we need to go further:
				if ( sPath.GetLength() > 3 )
				{
					// Call MakePath on parent directory....
					CStdString sParent;
					int iPos = sPath.ReverseFind( _T("\\") );
					if ( iPos >= 2 )
					{
						sParent = sPath.Left( iPos );
						hr = MakePath( sParent );
					}
					else
					{
						// else parent was root...
						hr = S_OK;
					}
					dwAttr = GetFileAttributes( sPath );
					if ( (int)dwAttr == (-1) )
					{
						if ( SUCCEEDED(hr) && CreateDirectory( sPath, NULL ) )
						{
							hr = S_OK;
						}
						else
						{
							hr = E_FAIL;
						}
					}
					else
					{
						if ( dwAttr & FILE_ATTRIBUTE_DIRECTORY )
						{
							hr = S_OK;
						}
						else
						{
							hr = E_FAIL;
						}
					}
				}
				else
				{
					//Drive root, just say it's "OK"
					hr = S_OK;
				}
			}
			// Else: Fail!
		}
		else if ( dwAttr & FILE_ATTRIBUTE_DIRECTORY )
		{
			// Already exists... that's fine.
			hr = S_OK;
		}
		// Else, failed. Must be file. We cannot make a path!
	}
	return hr;
}

CStdString D2Xutils::PathSlasher( LPCTSTR szPath, bool bSlashIt )
{
	CStdString sReturn;
	int iLen;

	if ( szPath && (iLen = _tcslen( szPath )) )
	{
		iLen--;
		if ( szPath[iLen] == _T('\\') )
		{
			if ( !bSlashIt )
			{
				sReturn = CStdString( szPath, iLen );
			}
			else
			{
				sReturn = szPath;
			}
		}
		else
		{
			if ( bSlashIt )
			{
				sReturn = szPath;
				sReturn += _T('\\');
			}
			else
			{
				sReturn = szPath;
			}
		}
		if ( !bSlashIt ) // Special case: Root directory MUST be slashed
		{
			if ( sReturn.GetLength() == 2 && sReturn[1] == _T(':') && _istalpha( sReturn[0]) )
			{
				sReturn += _T('\\');
			}
		}
	}
	return sReturn;
}

int D2Xutils::SetMediatype(const char* file,ULONG &mt,char* nmt)
{
	FILE *stream; 
	_XBE_CERTIFICATE HC;
	_XBE_HEADER HS;
	char rnmt[10];

	if(strlen(nmt) != 8)
		return 0;

	sprintf(rnmt,"%c%c%c%c%c%c%c%c",nmt[6],nmt[7],nmt[4],nmt[5],nmt[2],nmt[3],nmt[0],nmt[1]);

	stream  = fopen( file, "rb" );
	if(stream == NULL) 
		return 0;

	fread(&HS,1,sizeof(HS),stream);
	fseek(stream,HS.XbeHeaderSize,SEEK_SET);
	fread(&HC,1,sizeof(HC),stream);
	
	mt = HC.MediaTypes;
	fclose(stream);
	if(writeHex(file,rnmt,(HS.XbeHeaderSize + 156))!=0)
		return 0;
	return (HS.XbeHeaderSize + 156);
}


// emulated for smb
char* getenv(const char* szKey)
{
	return NULL;
}

void D2Xutils::getFatxName(char* pattern)
{
	int i=0;
	int fatx=42;
	char temp[60];
	strcpy(temp,"\0");
	while(pattern[i] != NULL)
	{
		if(isalnum(pattern[i]) || strchr(" !#$%&'()-.@[]^_`{}~",pattern[i]))
		{
			sprintf(temp,"%s%c",temp,pattern[i]);
		}
		++i;
	}
	memset(pattern,0,fatx+1);
	if(strlen(temp) > fatx)
	{
		char* c;
		c = strrchr(temp,'.');
		if(c)
		{
			strncpy(pattern,temp,fatx-strlen(c));
			strcat(pattern,c);
		} else {
			strncpy(pattern,temp,fatx);
			pattern[fatx] = '\0';
		}
	} else {
		strcpy(pattern,temp);
	}
	return;
}

void D2Xutils::getFatxName(WCHAR* pattern)
{
	char temp[90];
	wsprintf(temp,"%S",pattern);	
	getFatxName(temp);
	swprintf(pattern,L"%S", temp );
	return;
}

bool D2Xutils::isdriveD(char* path)
{
	if(!_strnicmp(path,"d:",2))
		return true;
	else
		return false;
}

bool D2Xutils::getfreeDiskspaceMB(char* drive,char* size)
{
	ULARGE_INTEGER ulFreeAvail;
	if( !GetDiskFreeSpaceEx( drive, NULL, NULL, &ulFreeAvail ) )
		return false;
	sprintf(size,"%6d MB free",(int)(ulFreeAvail.QuadPart/1048576));
	return true;
}

bool D2Xutils::getfreeDiskspaceMB(char* drive,int& size)
{
	ULARGE_INTEGER ulFreeAvail;
	if( !GetDiskFreeSpaceEx( drive, NULL, NULL, &ulFreeAvail ) )
		return false;
	size = int(ulFreeAvail.QuadPart/1048576);
	return true;
}

void D2Xutils::LaunchXbe(CHAR* szPath, CHAR* szXbe, CHAR* szParameters)
{
	CIoSupport helper;
	char temp[1024];
	if(!_strnicmp(szPath,"e:",2))
	{
		strcpy(temp,"Harddisk0\\Partition1");
		szPath+=2;
		strcat(temp,szPath);
	} else if(!_strnicmp(szPath,"f:",2))
	{
		strcpy(temp,"Harddisk0\\Partition6");
		szPath+=2;
		strcat(temp,szPath);
	} else if(!_strnicmp(szPath,"g:",2))
	{
		strcpy(temp,"Harddisk0\\Partition7");
		szPath+=2;
		strcat(temp,szPath);
	} else if(!_strnicmp(szPath,"d:",2))
	{
		strcpy(temp,"Cdrom0");
		szPath+=2;
		strcat(temp,szPath);
	}
	helper.Unmount("D:");
	helper.Mount("D:",temp);

	//DPf_H("Launching %s %s",temp,szXbe);

	if (szParameters==NULL)
	{
		XLaunchNewImage(szXbe, NULL );
	}
	else
	{
		LAUNCH_DATA LaunchData;
		strcpy((CHAR*)LaunchData.Data,szParameters);

		XLaunchNewImage(szXbe, &LaunchData );
	}
}


// XBMC
void D2Xutils::Unicode2Ansi(const wstring& wstrText,CStdString& strName)
{
  strName="";
  char *pstr=(char*)wstrText.c_str();
  for (int i=0; i < (int)wstrText.size();++i )
  {
    strName += pstr[i*2];
  }
}
