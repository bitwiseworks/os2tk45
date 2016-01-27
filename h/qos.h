#ifdef __cplusplus
   extern "C" {
#endif

#ifndef _QOS_H_
#define _QOS_H_

/*
 * macros to get desired or minimum qos
 * value from the component and macro to
 * get a composite from the two component
 */

#define DESIREDQOS(composite)           ((composite) >> 16)
#define MINIMUMQOS(composite)           ((composite) && 0x0000FFFF)
#define COMPOSITEQOS(desired,minimum)   (((desired)<<16)+(minimum))

/*
 * Quality of service values
 */
#define QOS_COMPLETE     100
#define QOS_NONE           1
#define QOS_DEFAULT        0
#define QOS_RESERVED     0xFF        /*values below this reserved */

/*
 * Composite quality of service values
 */
#define GUARANTEED       COMPOSITEQOS(QOS_COMPLETE, QOS_COMPLETE)
#define DONTCARE         COMPOSITEQOS(QOS_COMPLETE, QOS_NONE)
#define DONTRESERVE      COMPOSITEQOS(QOS_NONE, QOS_NONE)
#define SYSTEMDEFAULT    COMPOSITEQOS(QOS_DEFAULT, QOS_DEFAULT)

/*
 * How should qos errors be reported?
 */
#define ERROR_REPORT     1           /* report to the user */
#define ERROR_IGNORE     2           /* ignore them        */
#define ERROR_DEFAULT    0           /* according to the system setting */

/*
 * qos parameters
 */
#define SERVICE_REQUEST         1    /* service quality requested */
#define MAX_EE_JITTER           2    /* allowable jitter          */
#define MAX_DATA_RATE           3    /* maximum streaming rate    */
#define AVG_DATA_RATE           4    /* avg. streaming rate       */

/*
 * mmiom_beginstream parameters
 */
#define STREAM_READ             1
#define STREAM_WRITE            2

/*
 * qos structures - definitions
 */

typedef struct _QOS {                /* qos parameter          */
     LONG  lQOSParmId;               /* qos parameter name     */
     LONG  lQOSValue;                /* value of the parameter */
} QOS, *PQOS;
typedef struct _QOSInfo {
     LONG            lNumQOSParms;   /* number of qos parameters */
     QOS             QOSParms[1];    /* array of qos parameters  */
} QOSInfo, *PQOSInfo;

#endif /* _QOS_H_ */

#ifdef __cplusplus
}
#endif

