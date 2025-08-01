#include "functions.h"


#if defined(WIN32) || defined(_WIN32)
	#include <windows.h>
#else
	#include <unistd.h>

	#include <utime.h>
	#include <sys/time.h>
#endif

#include <sys/stat.h>

enum FileType
	{
	FileTypeUnknown			= 0x0,	///< File type is unknown
	FileTypeFile			= 0x1,	///< File is a file
	FileTypeFolder			= 0x2,	///< File is a folder or directory
	FileTypeDirectory		= 0x2,	///< File is a folder or directory
	FileTypeDevice			= 0x3,	///< File is a device
	FileTypeCharDevice		= 0x4,	///< File is a character device
	FileTypeBlockDevice		= 0x5,	///< File is a block device
	FileTypeSocket			= 0x6,	///< File is a socket
	FileTypeDoor			= 0x7,	///< File is a door
	FileTypeFifo			= 0x8,	///< File is a FIFO
	FileTypeXenixNamedFile	= 0x9,	///< File is a Xenix named file
	FileTypeNetworkSpecial	= 0xa,	///< File is a network special file.
	FileTypePipe			= 0xb,	///< File is a pipe
	FileTypeSymbolicLink	= 0xc	///< File is a symbolic link
	};


static int
internalGetFileType(const std::string &filename)
		{
		int		res=FileTypeUnknown;

#if defined(WIN32) || defined(_WIN32)
		uint32_t		winattrs = ::GetFileAttributesA(filename.c_str());

		if (winattrs != INVALID_FILE_ATTRIBUTES)
			{
			if ((winattrs & FILE_ATTRIBUTE_DIRECTORY) != 0) res = FileTypeDirectory;
			else if ((winattrs & FILE_ATTRIBUTE_DEVICE) != 0) res = FileTypeDevice;
			else res = FileTypeFile;
			}
#else
		uint32_t	unixmode=0, symlink=0;
		uint32_t	uid=0, gid=0;
		stat_t	st;

		if (lstat(SWString(filename.c_str()), &st) >= 0)
			{
#ifdef S_IFLNK
			if ((st.st_mode & S_IFMT) == S_IFLNK)
				{
				sw_stat_t	st2;

				if (sw_stat(filename.c_str(), &st2) >= 0)
					{
					// This is a symbolic link, BUT we want the info on the file it points to!
					// as we regard a symlink as an attribute not a file type
					symlink = 1;
					st = st2;
					}
				else
					{
					// this is a symbolic link, BUT it points to an invalid file
					// so we return the information on the link itself.
					symlink = 0;
					}
				}
#endif
			sw_uint32_t	unixmode = st.st_mode;

			switch (unixmode & S_IFMT)
				{
				case S_IFDIR:	 res = FileTypeDirectory;	break;
				case S_IFREG:	 res = FileTypeFile;	break;
	#ifdef S_IFBLK
				case S_IFBLK:	 res = FileTypeBlockDevice;	break;
	#endif
	#ifdef S_IFCHR
				case S_IFCHR:	 res = FileTypeCharDevice;	break;
	#endif
	#ifdef S_IFIFO
				case S_IFIFO:	 res = FileTypeFifo;	break;
	#endif
	#ifdef S_IFSOCK
				case S_IFSOCK:	 res = FileTypeSocket;	break;
	#endif
	#ifdef S_IFDOOR
				case S_IFDOOR:	 res = FileTypeDoor;	break;
	#endif
	#ifdef S_IFNAM
				case S_IFNAM:	
					//#define	S_IFNAM		0x5000  /* XENIX special named file */
					//#define	S_INSEM		0x1	/* XENIX semaphore subtype of IFNAM */
					//#define	S_INSHD		0x2	/* XENIX shared data subtype of IFNAM */
					res = FileTypeXenixNamedFile;
					break;
	#endif
	#ifdef S_IFLNK
				case S_IFLNK:	 res = FileTypeSymbolicLink;	break;
	#endif
				}
			}
#endif

		return res;
		}


bool
folder_exists(const std::string &path)
		{
		bool	res=false;

		return res;
		}

bool
path_isFile(const std::string &filename)
		{
		return (internalGetFileType(filename) == FileTypeFile);
		}


/// Returns true if the specified filename is a folder/directory.
bool
path_isDirectory(const std::string &filename)
		{
		return (internalGetFileType(filename) == FileTypeDirectory);
		}


/// Returns true if the specified filename is a folder/directory.
bool
path_isFolder(const std::string &filename)
		{
		return (internalGetFileType(filename) == FileTypeDirectory);
		}


