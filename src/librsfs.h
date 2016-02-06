
#ifndef RSFS_EXPORT_H
#define RSFS_EXPORT_H

#ifdef RSFS_BUILT_AS_STATIC
#  define RSFS_EXPORT
#  define LIBRSFS_NO_EXPORT
#else
#  ifndef RSFS_EXPORT
#    ifdef rsfs_EXPORTS
        /* We are building this library */
#      define RSFS_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define RSFS_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef LIBRSFS_NO_EXPORT
#    define LIBRSFS_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef LIBRSFS_DEPRECATED
#  define LIBRSFS_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef LIBRSFS_DEPRECATED_EXPORT
#  define LIBRSFS_DEPRECATED_EXPORT RSFS_EXPORT LIBRSFS_DEPRECATED
#endif

#ifndef LIBRSFS_DEPRECATED_NO_EXPORT
#  define LIBRSFS_DEPRECATED_NO_EXPORT LIBRSFS_NO_EXPORT LIBRSFS_DEPRECATED
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define LIBRSFS_NO_DEPRECATED
#endif

#endif
