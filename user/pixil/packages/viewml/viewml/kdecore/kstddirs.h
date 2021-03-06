/*
* kstddirs.h -- Declaration of class KStandardDirs.
* Generated by newclass on Thu Mar  5 16:05:28 EST 1998.
*/
#ifndef SSK_KSTDDIRS_H
#define SSK_KSTDDIRS_H

#include<qstring.h>
#include<qdict.h>

class QStrList;

/**
* Site-independent access to standard KDE directories.
*
* The various directory accessors allow you to access the various
* KDE FSSTND directories in a portable way.
*
* Since the KDE FSSTND is heirarchical, the accessors provide the
* ability to get the directory in the most to the least specific
* location (in order, Application/User/System);
*
* Additionally, each accessor has the ability to find the most
* specific directory that actually exists. The default is no check
* whether the directory exists.
* 
* Ordinarily, you will not want to manually instantiate this class.
* The global @ref KApplication object instantiates and provides const 
* access to a <code>KStandardDirs</code> object via the 
* @ref KApplication::dirs method.
*
* @author Sirtaj Singh Kang <taj@kde.org>
* @version $Id: kstddirs.h 1425 2006-10-03 11:27:15Z dslinux_amadeus $
*/
class KStandardDirs
{
public:
	/**
	* KStandardDirs Constructor.
	* @param appName The name of the application, which will be
	*		used for searching the "apps" directory.
	*/
	KStandardDirs( const char *appName );

	/**
	* KStandardDirs Destructor.
	*/
	~KStandardDirs();

	/** 
	 * The scope of a directory. App is the application-specific
	 * directory in the user's local KDE directory, User is the
	 * user's local KDE directory, SysApp is the application-specific
	 * directory in the system directory and System is the system
	 * directory in which KDE is installed.
	 *
	 * Closest is used for searching the first directory in the
	 * list that exists, in the order App, User, SysApp, System.
	 */
	enum DirScope { Closest, App, User, SysApp, System };

	/** 
	 * The full path to the application-specific directory.
	 */
	const char *app		(DirScope s = Closest) const;

	/** 
	 * The full path to the kde binary directory.
	 */
	const char *bin		(DirScope s = Closest) const;

	/** 
	 * The full path to the directory in which KDE-specific
	 * CGI programs are stored.
	 */
	const char *cgi		(DirScope s = Closest) const;

	/** 
	 * The full path to the directory in which configuration
	 * files are stored.
	 */
	const char *config	(DirScope s = Closest) const;

	/** 
	 * The full path to the data directory.
	 */
	const char *apps	(DirScope s = Closest) const;

	/** 
	 * The full path to the directory in which HTML documentation
	 * is saved. This does not take into account the current
	 * locale setting.
	 */
	const char *html	(DirScope s = Closest) const;

	/** 
	 * The full path to the directory in which icon images are stored.
	 */
	const char *icon	(DirScope s = Closest) const;

	/** 
	 * The full path to the directory in which locale information
	 * and translation catalogues are stored.
	 */
	const char *locale	(DirScope s = Closest) const;

	/** 
	 * The full path to the directory in which MIME information
	 * is stored.
	 */
	const char *mime	(DirScope s = Closest) const;

	/** 
	 * The full path to the directory in which mini icons are stored.
	 */
	const char *parts	(DirScope s = Closest) const;

	/** 
	 * The full path to the directory in which toolbar icons are
	 * stored.
	 */
	const char *toolbar	(DirScope s = Closest) const;

	/** 
	 * The full path to the directory in which wallpapers are stored.
	 */
	const char *wallpaper	(DirScope s = Closest) const;

	/** 
	 * The full path to the directory in which sound files are stored.
	 */
	const char *sound	(DirScope s = Closest) const;

	/** 
	* Finds the executable in the system path. A valid executable must
	* be a file and have its executable bit set.
	*
	* @see #findAllExe
	* @param appname the name of the executable file for which to search.
	* @param pathstr The path which will be searched. If this is 
	* 		0 (default), the $PATH environment variable will 
	*		be searched.
	* @param ignoreExecBit	If true, an existing file will be returned
	*			even if its executable bit is not set.
	*
	* @return The path of the executable. If it was not found, this string 
	*	will be null.
	*/
	static QString findExe( const char *appname, const char *pathstr=0,
			     bool ignoreExecBit=false );

	/** 
	 * Finds all occurences of an executable in the system path.
	 *
	 * @see	#findExe
	 *
	 * @param list	will be filled with the pathnames of all the
	 *		executables found. Will be empty if the executable
	 *		was not found.
	 * @param appname	The name of the executable for which to
	 *	 		search.
	 * @param pathstr	The path list which will be searched. If this
	 *		is 0 (default), the $PATH environment variable will
	 *		be searched.
	 * @param ignoreExecBit If true, an existing file will be returned
	 *			even if its executable bit is not set.
	 *
	 * @return The number of executables found, 0 if none were found.
	 */
	static int findAllExe( QStrList& list, const char *appname,
			const char *pathstr=0, bool ignoreExecBit=false );

private:

	/** 
	 * Finds a directory, subject to the suffix and scope.
	 */
	const char *closest( DirScope scope, const char *suffix ) const;



	/** Application name. */
	QString *_appName;
	const char *_appPath;

	// Directory dictionaries

	QDict<QString> *_app;
	QDict<QString> *_sysapp;
	QDict<QString> *_sys;
	QDict<QString> *_user;

	QString UserDir;
	const char * const KDEDir;

  // Disallow assignment and copy-construction
  KStandardDirs( const KStandardDirs& ) {};
  KStandardDirs& operator= ( const KStandardDirs& ) {};
};

#endif // SSK_KSTDDIRS_H
