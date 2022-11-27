/* Generated wolfSSL user_settings.h file for Arduino */
#ifndef ARDUINO_USER_SETTINGS_H
#define ARDUINO_USER_SETTINGS_H


/* Platform */
#define WOLFSSL_ARDUINO

/* Logging enabled */
//#define DEBUG_WOLFSSL

/* shows packet content */
//#define WOLFSSL_DEBUG_TLS

/* not used, screws up mallinfo() */
//#define WOLFSSL_TRACK_MEMORY
//#define WOLFSSL_DEBUG_MEMORY

/* Math library (remove this to use normal math)*/
#define USE_FAST_MATH
#define TFM_NO_ASM

/* RNG DEFAULT !!FOR TESTING ONLY!! */
/* comment out the error below to get started w/ bad entropy source
 * This will need fixed before distribution but is OK to test with */
//#error "needs solved, see: https://www.wolfssl.com/docs/porting-guide/"
#define WOLFSSL_GENSEED_FORTEST

#define WOLFSSL_GMTIME // Use the wolfSSL GMTIME solution
 #define USER_TICKS
 extern unsigned long my_time(unsigned long* timer);
 #define XTIME my_time
// Make sure that "my_time" returns second accuracy, doesn't have to be epoch related

// #define STATIC_CHUNKS_ONLY //verwijderd ivm buienradar weigering 

#define NO_ASN_TIME
#define SINGLE_THREADED 

#define XGMTIME
#define ALT_ECC_SIZE

#define NO_DH 

#define WOLFSSL_TLS12
#define HAVE_TLS_EXTENSIONS
#define HAVE_SUPPORTED_CURVES
#define HAVE_ECC
#define HAVE_HKDF
#define HAVE_FFDHE_8192 // or one of the other supported FFDHE sizes [2048, 3072, 4096, 6144, 8192]
#define WC_RSA_PSS
#define HAVE_AESGCM
#define HAVE_SNI

#define WOLFSSL_NO_ASM
#define TFM_NO_ASM
#endif /* ARDUINO_USER_SETTINGS_H */
