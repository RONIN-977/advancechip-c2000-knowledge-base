




























































































extern __cregister volatile unsigned int IFR;
extern __cregister volatile unsigned int IER;







 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 


 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 


 
# pragma diag_push
# pragma CHECK_MISRA("-19.7")
# pragma CHECK_MISRA("-19.4")
# pragma CHECK_MISRA("-19.1")
# pragma CHECK_MISRA("-19.15")
# pragma diag_pop

_Pragma("diag_push")
_Pragma("CHECK_MISRA(\"-19.4\")")
_Pragma("CHECK_MISRA(\"-19.1\")")
_Pragma("CHECK_MISRA(\"-19.6\")")



 



 

 
  







 



 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 


#pragma diag_push
#pragma CHECK_MISRA("-19.4")  

 


 
 
 

 
 
 

 
 
 

#pragma diag_pop


_Pragma("diag_pop")



_Pragma("diag_push")
_Pragma("CHECK_MISRA(\"-6.3\")")  
_Pragma("CHECK_MISRA(\"-19.4\")")  
_Pragma("CHECK_MISRA(\"-19.7\")")  
_Pragma("CHECK_MISRA(\"-19.13\")")  



extern  void _abort_msg(const char *msg);




_Pragma("diag_pop")


 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 




#pragma diag_push
#pragma CHECK_MISRA("-20.1")  
#pragma CHECK_MISRA("-20.2")  
#pragma CHECK_MISRA("-19.7")  
#pragma CHECK_MISRA("-19.10")  





























 





































 



_Pragma("diag_push")
_Pragma("CHECK_MISRA(\"none\")")



 






 









 












 











 





 





 







 







 









 








 







 


 







 




























 





 






 






 

 








 







 





























 

 

 

 




 











 

 





 



 






 












 


 

 

 

 

 

 

 

 

_Pragma("diag_pop")

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 



#pragma diag_push
 
#pragma CHECK_MISRA("-6.3")




 
    typedef          int    __int16_t;
    typedef unsigned int   __uint16_t;
    typedef          long   __int32_t;
    typedef unsigned long  __uint32_t;


 
typedef	long long		__int64_t;

 
typedef	unsigned long long	__uint64_t;



 
typedef	__uint32_t	__clock_t;		 
typedef	__int32_t	__critical_t;
typedef	double		__double_t;
typedef	float		__float_t;
typedef	__int32_t	__intfptr_t;
typedef	__int64_t	__intmax_t;
typedef __int32_t       __intptr_t;

typedef	__int16_t	__int_fast8_t;
typedef	__int16_t	__int_fast16_t;
typedef	__int32_t	__int_fast32_t;
typedef	__int64_t	__int_fast64_t;
typedef	__int16_t	__int_least8_t;
typedef	__int16_t	__int_least16_t;
typedef	__int32_t	__int_least32_t;
typedef	__int64_t	__int_least64_t;
typedef	long __ptrdiff_t;		 
typedef	__int16_t	__register_t;
typedef	__int32_t	__segsz_t;		 
typedef	unsigned long	__size_t;		 
typedef	__int32_t	__ssize_t;		 
typedef	__int64_t	__time_t;		 
typedef	__uint32_t	__uintfptr_t;
typedef	__uint64_t	__uintmax_t;
typedef	__uint32_t	__uintptr_t;

typedef	__uint16_t	__uint_fast8_t;
typedef	__uint16_t	__uint_fast16_t;
typedef	__uint32_t	__uint_fast32_t;
typedef	__uint64_t	__uint_fast64_t;
typedef	__uint16_t	__uint_least8_t;
typedef	__uint16_t	__uint_least16_t;
typedef	__uint32_t	__uint_least32_t;
typedef	__uint64_t	__uint_least64_t;
typedef	__uint16_t	__u_register_t;
typedef	__uint32_t	__vm_offset_t;
typedef	__uint32_t	__vm_paddr_t;
typedef	__uint32_t	__vm_size_t;

typedef	unsigned long ___wchar_t;




 
typedef long int _off_t;



 
typedef char* __va_list;

#pragma diag_pop


_Pragma("diag_push")
 
_Pragma("CHECK_MISRA(\"-6.3\")")



 
typedef	__int32_t	__blksize_t;	 
typedef	__int64_t	__blkcnt_t;	 
typedef	__int32_t	__clockid_t;	 
typedef	__uint32_t	__fflags_t;	 
typedef	__uint64_t	__fsblkcnt_t;
typedef	__uint64_t	__fsfilcnt_t;
typedef	__uint32_t	__gid_t;
typedef	__int64_t	__id_t;		 
typedef	__uint64_t	__ino_t;	 
typedef	long		__key_t;	 
typedef	__int32_t	__lwpid_t;	 
typedef	__uint16_t	__mode_t;	 
typedef	int		__accmode_t;	 
typedef	int		__nl_item;
typedef	__uint64_t	__nlink_t;	 
typedef	_off_t	        __off_t;	 
typedef	__int64_t	__off64_t;	 
typedef	__int32_t	__pid_t;	 
typedef	__int64_t	__rlim_t;	 
					 
					 
typedef	__uint16_t	__sa_family_t;
typedef	__uint32_t	__socklen_t;
typedef	long		__suseconds_t;	 
typedef	struct __timer	*__timer_t;	 
typedef	struct __mq	*__mqd_t;	 
typedef	__uint32_t	__uid_t;
typedef	unsigned int	__useconds_t;	 
typedef	int		__cpuwhich_t;	 
typedef	int		__cpulevel_t;	 
typedef int		__cpusetid_t;	 



 














 
typedef	unsigned long	__ct_rune_t;	 

typedef	__ct_rune_t	__rune_t;	 
typedef	__ct_rune_t	__wint_t;	 

 
typedef	__uint_least16_t __char16_t;
typedef	__uint_least32_t __char32_t;
 

typedef struct {
	long long __max_align1 __attribute__((aligned(__alignof__(long long))));
	long double __max_align2 __attribute__((aligned(__alignof__(long double))));
} __max_align_t;

typedef	__uint64_t	__dev_t;	 

typedef	__uint32_t	__fixpt_t;	 




 

typedef int _Mbstatet;

typedef _Mbstatet __mbstate_t;

typedef __uintmax_t     __rman_res_t;





 

_Pragma("diag_pop")


typedef __va_list va_list;




 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 

#pragma diag_pop






























 

 
 
 


_Pragma("diag_push")
_Pragma("CHECK_MISRA(\"-19.4\")")
_Pragma("CHECK_MISRA(\"-19.11\")")





_Pragma("diag_pop")

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 



_Pragma("diag_push")
_Pragma("CHECK_MISRA(\"-19.7\")")  
_Pragma("CHECK_MISRA(\"-20.1\")")  
_Pragma("CHECK_MISRA(\"-20.2\")")  



typedef long ptrdiff_t;


typedef unsigned long size_t;


typedef unsigned long wchar_t;


 
 
 
 
 
 
typedef long double max_align_t;

_Pragma("diag_push")
_Pragma("CHECK_MISRA(\"-19.10\")")  


_Pragma("diag_pop")


_Pragma("diag_pop")

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 


_Pragma("diag_push")
_Pragma("CHECK_MISRA(\"-19.1\")")  
_Pragma("CHECK_MISRA(\"-19.7\")")  

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 

_Pragma("diag_push")
_Pragma("CHECK_MISRA(\"-19.7\")")  






 



_Pragma("diag_pop")





























 




 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 

































 


#pragma diag_push
 
#pragma CHECK_MISRA("-19.4")
#pragma CHECK_MISRA("-19.7")
#pragma CHECK_MISRA("-19.13")










 
 

 

 




 
 

 

 




 
 

 

 




 




 




 
 

 

 

 


#pragma diag_pop






























 



typedef	__int16_t		int16_t;

typedef	__int32_t		int32_t;

typedef	__int64_t		int64_t;


typedef	__uint16_t		uint16_t;

typedef	__uint32_t		uint32_t;

typedef	__uint64_t		uint64_t;

typedef	__intptr_t		intptr_t;
typedef	__uintptr_t		uintptr_t;
typedef	__intmax_t		intmax_t;
typedef	__uintmax_t		uintmax_t;


typedef	__int_least8_t		int_least8_t;
typedef	__int_least16_t		int_least16_t;
typedef	__int_least32_t		int_least32_t;
typedef	__int_least64_t		int_least64_t;

typedef	__uint_least8_t		uint_least8_t;
typedef	__uint_least16_t	uint_least16_t;
typedef	__uint_least32_t	uint_least32_t;
typedef	__uint_least64_t	uint_least64_t;

typedef	__int_fast8_t		int_fast8_t;
typedef	__int_fast16_t		int_fast16_t;
typedef	__int_fast32_t		int_fast32_t;
typedef	__int_fast64_t		int_fast64_t;

typedef	__uint_fast8_t		uint_fast8_t;
typedef	__uint_fast16_t		uint_fast16_t;
typedef	__uint_fast32_t		uint_fast32_t;
typedef	__uint_fast64_t		uint_fast64_t;

_Pragma("diag_push")
_Pragma("CHECK_MISRA(\"-10.1\")")
 
_Pragma("diag_pop")

_Pragma("diag_push")
_Pragma("CHECK_MISRA(\"-19.4\")")
 
_Pragma("diag_pop")

 


_Pragma("diag_pop")










typedef _Bool bool_t;




typedef _Bool status_t;









typedef int                                     int16;
typedef long                                    int32;
typedef long long                               int64;
typedef unsigned int                            Uint16;
typedef unsigned long                           Uint32;
typedef unsigned long long                      Uint64;
typedef float         float32_t;
typedef double        float64_t;






typedef unsigned int bp_16 __attribute__((byte_peripheral));
typedef unsigned long bp_32 __attribute__((byte_peripheral));


















































struct ADCCTL1_BITS {                   
    Uint16 rsvd1:2;                     
    Uint16 INTPULSEPOS:1;               
    Uint16 rsvd2:4;                     
    Uint16 ADCPWDNZ:1;                  
    Uint16 ADCBSYCHN:4;                 
    Uint16 rsvd3:1;                     
    Uint16 ADCBSY:1;                    
    Uint16 rsvd4:2;                     
};

union ADCCTL1_REG {
    Uint16  all;
    struct  ADCCTL1_BITS  bit;
};

struct ADCCTL2_BITS {                   
    Uint16 PRESCALE:4;                  
    Uint16 rsvd1:2;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:5;                     
    Uint16 rsvd5:3;                     
};

union ADCCTL2_REG {
    Uint16  all;
    struct  ADCCTL2_BITS  bit;
};

struct ADCBURSTCTL_BITS {               
    Uint16 BURSTTRIGSEL:6;              
    Uint16 rsvd1:2;                     
    Uint16 BURSTSIZE:4;                 
    Uint16 rsvd2:3;                     
    Uint16 BURSTEN:1;                   
};

union ADCBURSTCTL_REG {
    Uint16  all;
    struct  ADCBURSTCTL_BITS  bit;
};

struct ADCINTFLG_BITS {                 
    Uint16 ADCINT1:1;                   
    Uint16 ADCINT2:1;                   
    Uint16 ADCINT3:1;                   
    Uint16 ADCINT4:1;                   
    Uint16 rsvd1:12;                    
};

union ADCINTFLG_REG {
    Uint16  all;
    struct  ADCINTFLG_BITS  bit;
};

struct ADCINTFLGCLR_BITS {              
    Uint16 ADCINT1:1;                   
    Uint16 ADCINT2:1;                   
    Uint16 ADCINT3:1;                   
    Uint16 ADCINT4:1;                   
    Uint16 rsvd1:12;                    
};

union ADCINTFLGCLR_REG {
    Uint16  all;
    struct  ADCINTFLGCLR_BITS  bit;
};

struct ADCINTOVF_BITS {                 
    Uint16 ADCINT1:1;                   
    Uint16 ADCINT2:1;                   
    Uint16 ADCINT3:1;                   
    Uint16 ADCINT4:1;                   
    Uint16 rsvd1:12;                    
};

union ADCINTOVF_REG {
    Uint16  all;
    struct  ADCINTOVF_BITS  bit;
};

struct ADCINTOVFCLR_BITS {              
    Uint16 ADCINT1:1;                   
    Uint16 ADCINT2:1;                   
    Uint16 ADCINT3:1;                   
    Uint16 ADCINT4:1;                   
    Uint16 rsvd1:12;                    
};

union ADCINTOVFCLR_REG {
    Uint16  all;
    struct  ADCINTOVFCLR_BITS  bit;
};

struct ADCINTSEL1N2_BITS {              
    Uint16 INT1SEL:4;                   
    Uint16 rsvd1:1;                     
    Uint16 INT1E:1;                     
    Uint16 INT1CONT:1;                  
    Uint16 rsvd2:1;                     
    Uint16 INT2SEL:4;                   
    Uint16 rsvd3:1;                     
    Uint16 INT2E:1;                     
    Uint16 INT2CONT:1;                  
    Uint16 rsvd4:1;                     
};

union ADCINTSEL1N2_REG {
    Uint16  all;
    struct  ADCINTSEL1N2_BITS  bit;
};

struct ADCINTSEL3N4_BITS {              
    Uint16 INT3SEL:4;                   
    Uint16 rsvd1:1;                     
    Uint16 INT3E:1;                     
    Uint16 INT3CONT:1;                  
    Uint16 rsvd2:1;                     
    Uint16 INT4SEL:4;                   
    Uint16 rsvd3:1;                     
    Uint16 INT4E:1;                     
    Uint16 INT4CONT:1;                  
    Uint16 rsvd4:1;                     
};

union ADCINTSEL3N4_REG {
    Uint16  all;
    struct  ADCINTSEL3N4_BITS  bit;
};

struct ADCSOCPRICTL_BITS {              
    Uint16 SOCPRIORITY:5;               
    Uint16 RRPOINTER:5;                 
    Uint16 rsvd1:6;                     
};

union ADCSOCPRICTL_REG {
    Uint16  all;
    struct  ADCSOCPRICTL_BITS  bit;
};

struct ADCINTSOCSEL1_BITS {             
    Uint16 SOC0:2;                      
    Uint16 SOC1:2;                      
    Uint16 SOC2:2;                      
    Uint16 SOC3:2;                      
    Uint16 SOC4:2;                      
    Uint16 SOC5:2;                      
    Uint16 SOC6:2;                      
    Uint16 SOC7:2;                      
};

union ADCINTSOCSEL1_REG {
    Uint16  all;
    struct  ADCINTSOCSEL1_BITS  bit;
};

struct ADCINTSOCSEL2_BITS {             
    Uint16 SOC8:2;                      
    Uint16 SOC9:2;                      
    Uint16 SOC10:2;                     
    Uint16 SOC11:2;                     
    Uint16 SOC12:2;                     
    Uint16 SOC13:2;                     
    Uint16 SOC14:2;                     
    Uint16 SOC15:2;                     
};

union ADCINTSOCSEL2_REG {
    Uint16  all;
    struct  ADCINTSOCSEL2_BITS  bit;
};

struct ADCSOCFLG1_BITS {                
    Uint16 SOC0:1;                      
    Uint16 SOC1:1;                      
    Uint16 SOC2:1;                      
    Uint16 SOC3:1;                      
    Uint16 SOC4:1;                      
    Uint16 SOC5:1;                      
    Uint16 SOC6:1;                      
    Uint16 SOC7:1;                      
    Uint16 SOC8:1;                      
    Uint16 SOC9:1;                      
    Uint16 SOC10:1;                     
    Uint16 SOC11:1;                     
    Uint16 SOC12:1;                     
    Uint16 SOC13:1;                     
    Uint16 SOC14:1;                     
    Uint16 SOC15:1;                     
};

union ADCSOCFLG1_REG {
    Uint16  all;
    struct  ADCSOCFLG1_BITS  bit;
};

struct ADCSOCFRC1_BITS {                
    Uint16 SOC0:1;                      
    Uint16 SOC1:1;                      
    Uint16 SOC2:1;                      
    Uint16 SOC3:1;                      
    Uint16 SOC4:1;                      
    Uint16 SOC5:1;                      
    Uint16 SOC6:1;                      
    Uint16 SOC7:1;                      
    Uint16 SOC8:1;                      
    Uint16 SOC9:1;                      
    Uint16 SOC10:1;                     
    Uint16 SOC11:1;                     
    Uint16 SOC12:1;                     
    Uint16 SOC13:1;                     
    Uint16 SOC14:1;                     
    Uint16 SOC15:1;                     
};

union ADCSOCFRC1_REG {
    Uint16  all;
    struct  ADCSOCFRC1_BITS  bit;
};

struct ADCSOCOVF1_BITS {                
    Uint16 SOC0:1;                      
    Uint16 SOC1:1;                      
    Uint16 SOC2:1;                      
    Uint16 SOC3:1;                      
    Uint16 SOC4:1;                      
    Uint16 SOC5:1;                      
    Uint16 SOC6:1;                      
    Uint16 SOC7:1;                      
    Uint16 SOC8:1;                      
    Uint16 SOC9:1;                      
    Uint16 SOC10:1;                     
    Uint16 SOC11:1;                     
    Uint16 SOC12:1;                     
    Uint16 SOC13:1;                     
    Uint16 SOC14:1;                     
    Uint16 SOC15:1;                     
};

union ADCSOCOVF1_REG {
    Uint16  all;
    struct  ADCSOCOVF1_BITS  bit;
};

struct ADCSOCOVFCLR1_BITS {             
    Uint16 SOC0:1;                      
    Uint16 SOC1:1;                      
    Uint16 SOC2:1;                      
    Uint16 SOC3:1;                      
    Uint16 SOC4:1;                      
    Uint16 SOC5:1;                      
    Uint16 SOC6:1;                      
    Uint16 SOC7:1;                      
    Uint16 SOC8:1;                      
    Uint16 SOC9:1;                      
    Uint16 SOC10:1;                     
    Uint16 SOC11:1;                     
    Uint16 SOC12:1;                     
    Uint16 SOC13:1;                     
    Uint16 SOC14:1;                     
    Uint16 SOC15:1;                     
};

union ADCSOCOVFCLR1_REG {
    Uint16  all;
    struct  ADCSOCOVFCLR1_BITS  bit;
};

struct ADCSOC0CTL_BITS {                
    Uint16 ACQPS:9;                     
    Uint16 rsvd1:6;                     
    Uint32 CHSEL:4;                     
    Uint16 rsvd2:1;                     
    Uint16 TRIGSEL:5;                   
    Uint16 rsvd3:7;                     
};

union ADCSOC0CTL_REG {
    Uint32  all;
    struct  ADCSOC0CTL_BITS  bit;
};

struct ADCSOC1CTL_BITS {                
    Uint16 ACQPS:9;                     
    Uint16 rsvd1:6;                     
    Uint32 CHSEL:4;                     
    Uint16 rsvd2:1;                     
    Uint16 TRIGSEL:5;                   
    Uint16 rsvd3:7;                     
};

union ADCSOC1CTL_REG {
    Uint32  all;
    struct  ADCSOC1CTL_BITS  bit;
};

struct ADCSOC2CTL_BITS {                
    Uint16 ACQPS:9;                     
    Uint16 rsvd1:6;                     
    Uint32 CHSEL:4;                     
    Uint16 rsvd2:1;                     
    Uint16 TRIGSEL:5;                   
    Uint16 rsvd3:7;                     
};

union ADCSOC2CTL_REG {
    Uint32  all;
    struct  ADCSOC2CTL_BITS  bit;
};

struct ADCSOC3CTL_BITS {                
    Uint16 ACQPS:9;                     
    Uint16 rsvd1:6;                     
    Uint32 CHSEL:4;                     
    Uint16 rsvd2:1;                     
    Uint16 TRIGSEL:5;                   
    Uint16 rsvd3:7;                     
};

union ADCSOC3CTL_REG {
    Uint32  all;
    struct  ADCSOC3CTL_BITS  bit;
};

struct ADCSOC4CTL_BITS {                
    Uint16 ACQPS:9;                     
    Uint16 rsvd1:6;                     
    Uint32 CHSEL:4;                     
    Uint16 rsvd2:1;                     
    Uint16 TRIGSEL:5;                   
    Uint16 rsvd3:7;                     
};

union ADCSOC4CTL_REG {
    Uint32  all;
    struct  ADCSOC4CTL_BITS  bit;
};

struct ADCSOC5CTL_BITS {                
    Uint16 ACQPS:9;                     
    Uint16 rsvd1:6;                     
    Uint32 CHSEL:4;                     
    Uint16 rsvd2:1;                     
    Uint16 TRIGSEL:5;                   
    Uint16 rsvd3:7;                     
};

union ADCSOC5CTL_REG {
    Uint32  all;
    struct  ADCSOC5CTL_BITS  bit;
};

struct ADCSOC6CTL_BITS {                
    Uint16 ACQPS:9;                     
    Uint16 rsvd1:6;                     
    Uint32 CHSEL:4;                     
    Uint16 rsvd2:1;                     
    Uint16 TRIGSEL:5;                   
    Uint16 rsvd3:7;                     
};

union ADCSOC6CTL_REG {
    Uint32  all;
    struct  ADCSOC6CTL_BITS  bit;
};

struct ADCSOC7CTL_BITS {                
    Uint16 ACQPS:9;                     
    Uint16 rsvd1:6;                     
    Uint32 CHSEL:4;                     
    Uint16 rsvd2:1;                     
    Uint16 TRIGSEL:5;                   
    Uint16 rsvd3:7;                     
};

union ADCSOC7CTL_REG {
    Uint32  all;
    struct  ADCSOC7CTL_BITS  bit;
};

struct ADCSOC8CTL_BITS {                
    Uint16 ACQPS:9;                     
    Uint16 rsvd1:6;                     
    Uint32 CHSEL:4;                     
    Uint16 rsvd2:1;                     
    Uint16 TRIGSEL:5;                   
    Uint16 rsvd3:7;                     
};

union ADCSOC8CTL_REG {
    Uint32  all;
    struct  ADCSOC8CTL_BITS  bit;
};

struct ADCSOC9CTL_BITS {                
    Uint16 ACQPS:9;                     
    Uint16 rsvd1:6;                     
    Uint32 CHSEL:4;                     
    Uint16 rsvd2:1;                     
    Uint16 TRIGSEL:5;                   
    Uint16 rsvd3:7;                     
};

union ADCSOC9CTL_REG {
    Uint32  all;
    struct  ADCSOC9CTL_BITS  bit;
};

struct ADCSOC10CTL_BITS {               
    Uint16 ACQPS:9;                     
    Uint16 rsvd1:6;                     
    Uint32 CHSEL:4;                     
    Uint16 rsvd2:1;                     
    Uint16 TRIGSEL:5;                   
    Uint16 rsvd3:7;                     
};

union ADCSOC10CTL_REG {
    Uint32  all;
    struct  ADCSOC10CTL_BITS  bit;
};

struct ADCSOC11CTL_BITS {               
    Uint16 ACQPS:9;                     
    Uint16 rsvd1:6;                     
    Uint32 CHSEL:4;                     
    Uint16 rsvd2:1;                     
    Uint16 TRIGSEL:5;                   
    Uint16 rsvd3:7;                     
};

union ADCSOC11CTL_REG {
    Uint32  all;
    struct  ADCSOC11CTL_BITS  bit;
};

struct ADCSOC12CTL_BITS {               
    Uint16 ACQPS:9;                     
    Uint16 rsvd1:6;                     
    Uint32 CHSEL:4;                     
    Uint16 rsvd2:1;                     
    Uint16 TRIGSEL:5;                   
    Uint16 rsvd3:7;                     
};

union ADCSOC12CTL_REG {
    Uint32  all;
    struct  ADCSOC12CTL_BITS  bit;
};

struct ADCSOC13CTL_BITS {               
    Uint16 ACQPS:9;                     
    Uint16 rsvd1:6;                     
    Uint32 CHSEL:4;                     
    Uint16 rsvd2:1;                     
    Uint16 TRIGSEL:5;                   
    Uint16 rsvd3:7;                     
};

union ADCSOC13CTL_REG {
    Uint32  all;
    struct  ADCSOC13CTL_BITS  bit;
};

struct ADCSOC14CTL_BITS {               
    Uint16 ACQPS:9;                     
    Uint16 rsvd1:6;                     
    Uint32 CHSEL:4;                     
    Uint16 rsvd2:1;                     
    Uint16 TRIGSEL:5;                   
    Uint16 rsvd3:7;                     
};

union ADCSOC14CTL_REG {
    Uint32  all;
    struct  ADCSOC14CTL_BITS  bit;
};

struct ADCSOC15CTL_BITS {               
    Uint16 ACQPS:9;                     
    Uint16 rsvd1:6;                     
    Uint32 CHSEL:4;                     
    Uint16 rsvd2:1;                     
    Uint16 TRIGSEL:5;                   
    Uint16 rsvd3:7;                     
};

union ADCSOC15CTL_REG {
    Uint32  all;
    struct  ADCSOC15CTL_BITS  bit;
};

struct ADCEVTSTAT_BITS {                
    Uint16 PPB1TRIPHI:1;                
    Uint16 PPB1TRIPLO:1;                
    Uint16 PPB1ZERO:1;                  
    Uint16 rsvd1:1;                     
    Uint16 PPB2TRIPHI:1;                
    Uint16 PPB2TRIPLO:1;                
    Uint16 PPB2ZERO:1;                  
    Uint16 rsvd2:1;                     
    Uint16 PPB3TRIPHI:1;                
    Uint16 PPB3TRIPLO:1;                
    Uint16 PPB3ZERO:1;                  
    Uint16 rsvd3:1;                     
    Uint16 PPB4TRIPHI:1;                
    Uint16 PPB4TRIPLO:1;                
    Uint16 PPB4ZERO:1;                  
    Uint16 rsvd4:1;                     
};

union ADCEVTSTAT_REG {
    Uint16  all;
    struct  ADCEVTSTAT_BITS  bit;
};

struct ADCEVTCLR_BITS {                 
    Uint16 PPB1TRIPHI:1;                
    Uint16 PPB1TRIPLO:1;                
    Uint16 PPB1ZERO:1;                  
    Uint16 rsvd1:1;                     
    Uint16 PPB2TRIPHI:1;                
    Uint16 PPB2TRIPLO:1;                
    Uint16 PPB2ZERO:1;                  
    Uint16 rsvd2:1;                     
    Uint16 PPB3TRIPHI:1;                
    Uint16 PPB3TRIPLO:1;                
    Uint16 PPB3ZERO:1;                  
    Uint16 rsvd3:1;                     
    Uint16 PPB4TRIPHI:1;                
    Uint16 PPB4TRIPLO:1;                
    Uint16 PPB4ZERO:1;                  
    Uint16 rsvd4:1;                     
};

union ADCEVTCLR_REG {
    Uint16  all;
    struct  ADCEVTCLR_BITS  bit;
};

struct ADCEVTSEL_BITS {                 
    Uint16 PPB1TRIPHI:1;                
    Uint16 PPB1TRIPLO:1;                
    Uint16 PPB1ZERO:1;                  
    Uint16 rsvd1:1;                     
    Uint16 PPB2TRIPHI:1;                
    Uint16 PPB2TRIPLO:1;                
    Uint16 PPB2ZERO:1;                  
    Uint16 rsvd2:1;                     
    Uint16 PPB3TRIPHI:1;                
    Uint16 PPB3TRIPLO:1;                
    Uint16 PPB3ZERO:1;                  
    Uint16 rsvd3:1;                     
    Uint16 PPB4TRIPHI:1;                
    Uint16 PPB4TRIPLO:1;                
    Uint16 PPB4ZERO:1;                  
    Uint16 rsvd4:1;                     
};

union ADCEVTSEL_REG {
    Uint16  all;
    struct  ADCEVTSEL_BITS  bit;
};

struct ADCEVTINTSEL_BITS {              
    Uint16 PPB1TRIPHI:1;                
    Uint16 PPB1TRIPLO:1;                
    Uint16 PPB1ZERO:1;                  
    Uint16 rsvd1:1;                     
    Uint16 PPB2TRIPHI:1;                
    Uint16 PPB2TRIPLO:1;                
    Uint16 PPB2ZERO:1;                  
    Uint16 rsvd2:1;                     
    Uint16 PPB3TRIPHI:1;                
    Uint16 PPB3TRIPLO:1;                
    Uint16 PPB3ZERO:1;                  
    Uint16 rsvd3:1;                     
    Uint16 PPB4TRIPHI:1;                
    Uint16 PPB4TRIPLO:1;                
    Uint16 PPB4ZERO:1;                  
    Uint16 rsvd4:1;                     
};

union ADCEVTINTSEL_REG {
    Uint16  all;
    struct  ADCEVTINTSEL_BITS  bit;
};

struct ADCOSDETECT_BITS {               
    Uint16 DETECTCFG:3;                 
    Uint16 rsvd1:13;                    
};

union ADCOSDETECT_REG {
    Uint16  all;
    struct  ADCOSDETECT_BITS  bit;
};

struct ADCCOUNTER_BITS {                
    Uint16 FREECOUNT:12;                
    Uint16 rsvd1:4;                     
};

union ADCCOUNTER_REG {
    Uint16  all;
    struct  ADCCOUNTER_BITS  bit;
};

struct ADCREV_BITS {                    
    Uint16 TYPE:8;                      
    Uint16 REV:8;                       
};

union ADCREV_REG {
    Uint16  all;
    struct  ADCREV_BITS  bit;
};

struct ADCOFFTRIM_BITS {                
    Uint16 OFFTRIM:8;                   
    Uint16 rsvd1:4;                     
    Uint16 rsvd2:4;                     
};

union ADCOFFTRIM_REG {
    Uint16  all;
    struct  ADCOFFTRIM_BITS  bit;
};

struct ADCPPB1CONFIG_BITS {             
    Uint16 CONFIG:4;                    
    Uint16 TWOSCOMPEN:1;                
    Uint16 CBCEN:1;                     
    Uint16 rsvd1:10;                    
};

union ADCPPB1CONFIG_REG {
    Uint16  all;
    struct  ADCPPB1CONFIG_BITS  bit;
};

struct ADCPPB1STAMP_BITS {              
    Uint16 DLYSTAMP:12;                 
    Uint16 rsvd1:4;                     
};

union ADCPPB1STAMP_REG {
    Uint16  all;
    struct  ADCPPB1STAMP_BITS  bit;
};

struct ADCPPB1OFFCAL_BITS {             
    Uint16 OFFCAL:10;                   
    Uint16 rsvd1:6;                     
};

union ADCPPB1OFFCAL_REG {
    Uint16  all;
    struct  ADCPPB1OFFCAL_BITS  bit;
};

struct ADCPPB1TRIPHI_BITS {             
    Uint16 LIMITHI:16;                  
    Uint16 HSIGN:1;                     
    Uint16 rsvd1:15;                    
};

union ADCPPB1TRIPHI_REG {
    Uint32  all;
    struct  ADCPPB1TRIPHI_BITS  bit;
};

struct ADCPPB1TRIPLO_BITS {             
    Uint16 LIMITLO:16;                  
    Uint16 LSIGN:1;                     
    Uint16 rsvd1:3;                     
    Uint16 REQSTAMP:12;                 
};

union ADCPPB1TRIPLO_REG {
    Uint32  all;
    struct  ADCPPB1TRIPLO_BITS  bit;
};

struct ADCPPB2CONFIG_BITS {             
    Uint16 CONFIG:4;                    
    Uint16 TWOSCOMPEN:1;                
    Uint16 CBCEN:1;                     
    Uint16 rsvd1:10;                    
};

union ADCPPB2CONFIG_REG {
    Uint16  all;
    struct  ADCPPB2CONFIG_BITS  bit;
};

struct ADCPPB2STAMP_BITS {              
    Uint16 DLYSTAMP:12;                 
    Uint16 rsvd1:4;                     
};

union ADCPPB2STAMP_REG {
    Uint16  all;
    struct  ADCPPB2STAMP_BITS  bit;
};

struct ADCPPB2OFFCAL_BITS {             
    Uint16 OFFCAL:10;                   
    Uint16 rsvd1:6;                     
};

union ADCPPB2OFFCAL_REG {
    Uint16  all;
    struct  ADCPPB2OFFCAL_BITS  bit;
};

struct ADCPPB2TRIPHI_BITS {             
    Uint16 LIMITHI:16;                  
    Uint16 HSIGN:1;                     
    Uint16 rsvd1:15;                    
};

union ADCPPB2TRIPHI_REG {
    Uint32  all;
    struct  ADCPPB2TRIPHI_BITS  bit;
};

struct ADCPPB2TRIPLO_BITS {             
    Uint16 LIMITLO:16;                  
    Uint16 LSIGN:1;                     
    Uint16 rsvd1:3;                     
    Uint16 REQSTAMP:12;                 
};

union ADCPPB2TRIPLO_REG {
    Uint32  all;
    struct  ADCPPB2TRIPLO_BITS  bit;
};

struct ADCPPB3CONFIG_BITS {             
    Uint16 CONFIG:4;                    
    Uint16 TWOSCOMPEN:1;                
    Uint16 CBCEN:1;                     
    Uint16 rsvd1:10;                    
};

union ADCPPB3CONFIG_REG {
    Uint16  all;
    struct  ADCPPB3CONFIG_BITS  bit;
};

struct ADCPPB3STAMP_BITS {              
    Uint16 DLYSTAMP:12;                 
    Uint16 rsvd1:4;                     
};

union ADCPPB3STAMP_REG {
    Uint16  all;
    struct  ADCPPB3STAMP_BITS  bit;
};

struct ADCPPB3OFFCAL_BITS {             
    Uint16 OFFCAL:10;                   
    Uint16 rsvd1:6;                     
};

union ADCPPB3OFFCAL_REG {
    Uint16  all;
    struct  ADCPPB3OFFCAL_BITS  bit;
};

struct ADCPPB3TRIPHI_BITS {             
    Uint16 LIMITHI:16;                  
    Uint16 HSIGN:1;                     
    Uint16 rsvd1:15;                    
};

union ADCPPB3TRIPHI_REG {
    Uint32  all;
    struct  ADCPPB3TRIPHI_BITS  bit;
};

struct ADCPPB3TRIPLO_BITS {             
    Uint16 LIMITLO:16;                  
    Uint16 LSIGN:1;                     
    Uint16 rsvd1:3;                     
    Uint16 REQSTAMP:12;                 
};

union ADCPPB3TRIPLO_REG {
    Uint32  all;
    struct  ADCPPB3TRIPLO_BITS  bit;
};

struct ADCPPB4CONFIG_BITS {             
    Uint16 CONFIG:4;                    
    Uint16 TWOSCOMPEN:1;                
    Uint16 CBCEN:1;                     
    Uint16 rsvd1:10;                    
};

union ADCPPB4CONFIG_REG {
    Uint16  all;
    struct  ADCPPB4CONFIG_BITS  bit;
};

struct ADCPPB4STAMP_BITS {              
    Uint16 DLYSTAMP:12;                 
    Uint16 rsvd1:4;                     
};

union ADCPPB4STAMP_REG {
    Uint16  all;
    struct  ADCPPB4STAMP_BITS  bit;
};

struct ADCPPB4OFFCAL_BITS {             
    Uint16 OFFCAL:10;                   
    Uint16 rsvd1:6;                     
};

union ADCPPB4OFFCAL_REG {
    Uint16  all;
    struct  ADCPPB4OFFCAL_BITS  bit;
};

struct ADCPPB4TRIPHI_BITS {             
    Uint16 LIMITHI:16;                  
    Uint16 HSIGN:1;                     
    Uint16 rsvd1:15;                    
};

union ADCPPB4TRIPHI_REG {
    Uint32  all;
    struct  ADCPPB4TRIPHI_BITS  bit;
};

struct ADCPPB4TRIPLO_BITS {             
    Uint16 LIMITLO:16;                  
    Uint16 LSIGN:1;                     
    Uint16 rsvd1:3;                     
    Uint16 REQSTAMP:12;                 
};

union ADCPPB4TRIPLO_REG {
    Uint32  all;
    struct  ADCPPB4TRIPLO_BITS  bit;
};

struct ADC_REGS {
    union   ADCCTL1_REG                      ADCCTL1;                      
    union   ADCCTL2_REG                      ADCCTL2;                      
    union   ADCBURSTCTL_REG                  ADCBURSTCTL;                  
    union   ADCINTFLG_REG                    ADCINTFLG;                    
    union   ADCINTFLGCLR_REG                 ADCINTFLGCLR;                 
    union   ADCINTOVF_REG                    ADCINTOVF;                    
    union   ADCINTOVFCLR_REG                 ADCINTOVFCLR;                 
    union   ADCINTSEL1N2_REG                 ADCINTSEL1N2;                 
    union   ADCINTSEL3N4_REG                 ADCINTSEL3N4;                 
    union   ADCSOCPRICTL_REG                 ADCSOCPRICTL;                 
    union   ADCINTSOCSEL1_REG                ADCINTSOCSEL1;                
    union   ADCINTSOCSEL2_REG                ADCINTSOCSEL2;                
    union   ADCSOCFLG1_REG                   ADCSOCFLG1;                   
    union   ADCSOCFRC1_REG                   ADCSOCFRC1;                   
    union   ADCSOCOVF1_REG                   ADCSOCOVF1;                   
    union   ADCSOCOVFCLR1_REG                ADCSOCOVFCLR1;                
    union   ADCSOC0CTL_REG                   ADCSOC0CTL;                   
    union   ADCSOC1CTL_REG                   ADCSOC1CTL;                   
    union   ADCSOC2CTL_REG                   ADCSOC2CTL;                   
    union   ADCSOC3CTL_REG                   ADCSOC3CTL;                   
    union   ADCSOC4CTL_REG                   ADCSOC4CTL;                   
    union   ADCSOC5CTL_REG                   ADCSOC5CTL;                   
    union   ADCSOC6CTL_REG                   ADCSOC6CTL;                   
    union   ADCSOC7CTL_REG                   ADCSOC7CTL;                   
    union   ADCSOC8CTL_REG                   ADCSOC8CTL;                   
    union   ADCSOC9CTL_REG                   ADCSOC9CTL;                   
    union   ADCSOC10CTL_REG                  ADCSOC10CTL;                  
    union   ADCSOC11CTL_REG                  ADCSOC11CTL;                  
    union   ADCSOC12CTL_REG                  ADCSOC12CTL;                  
    union   ADCSOC13CTL_REG                  ADCSOC13CTL;                  
    union   ADCSOC14CTL_REG                  ADCSOC14CTL;                  
    union   ADCSOC15CTL_REG                  ADCSOC15CTL;                  
    union   ADCEVTSTAT_REG                   ADCEVTSTAT;                   
    Uint16                                   rsvd1;                        
    union   ADCEVTCLR_REG                    ADCEVTCLR;                    
    Uint16                                   rsvd2;                        
    union   ADCEVTSEL_REG                    ADCEVTSEL;                    
    Uint16                                   rsvd3;                        
    union   ADCEVTINTSEL_REG                 ADCEVTINTSEL;                 
    Uint16                                   rsvd4;                        
    union   ADCOSDETECT_REG                  ADCOSDETECT;                  
    union   ADCCOUNTER_REG                   ADCCOUNTER;                   
    union   ADCREV_REG                       ADCREV;                       
    union   ADCOFFTRIM_REG                   ADCOFFTRIM;                   
    Uint16                                   rsvd5[4];                     
    union   ADCPPB1CONFIG_REG                ADCPPB1CONFIG;                
    union   ADCPPB1STAMP_REG                 ADCPPB1STAMP;                 
    union   ADCPPB1OFFCAL_REG                ADCPPB1OFFCAL;                
    Uint16                                   ADCPPB1OFFREF;                
    union   ADCPPB1TRIPHI_REG                ADCPPB1TRIPHI;                
    union   ADCPPB1TRIPLO_REG                ADCPPB1TRIPLO;                
    union   ADCPPB2CONFIG_REG                ADCPPB2CONFIG;                
    union   ADCPPB2STAMP_REG                 ADCPPB2STAMP;                 
    union   ADCPPB2OFFCAL_REG                ADCPPB2OFFCAL;                
    Uint16                                   ADCPPB2OFFREF;                
    union   ADCPPB2TRIPHI_REG                ADCPPB2TRIPHI;                
    union   ADCPPB2TRIPLO_REG                ADCPPB2TRIPLO;                
    union   ADCPPB3CONFIG_REG                ADCPPB3CONFIG;                
    union   ADCPPB3STAMP_REG                 ADCPPB3STAMP;                 
    union   ADCPPB3OFFCAL_REG                ADCPPB3OFFCAL;                
    Uint16                                   ADCPPB3OFFREF;                
    union   ADCPPB3TRIPHI_REG                ADCPPB3TRIPHI;                
    union   ADCPPB3TRIPLO_REG                ADCPPB3TRIPLO;                
    union   ADCPPB4CONFIG_REG                ADCPPB4CONFIG;                
    union   ADCPPB4STAMP_REG                 ADCPPB4STAMP;                 
    union   ADCPPB4OFFCAL_REG                ADCPPB4OFFCAL;                
    Uint16                                   ADCPPB4OFFREF;                
    union   ADCPPB4TRIPHI_REG                ADCPPB4TRIPHI;                
    union   ADCPPB4TRIPLO_REG                ADCPPB4TRIPLO;                
    Uint16                                   rsvd6[15];                    
    Uint16                                   ADCINTCYCLE;                  
    Uint16                                   rsvd7[2];                     
    Uint32                                   ADCINLTRIM2;                  
    Uint32                                   ADCINLTRIM3;                  
};

struct ADCPPB1RESULT_BITS {             
    Uint16 PPBRESULT:16;                
    Uint16 SIGN:16;                     
};

union ADCPPB1RESULT_REG {
    Uint32  all;
    struct  ADCPPB1RESULT_BITS  bit;
};

struct ADCPPB2RESULT_BITS {             
    Uint16 PPBRESULT:16;                
    Uint16 SIGN:16;                     
};

union ADCPPB2RESULT_REG {
    Uint32  all;
    struct  ADCPPB2RESULT_BITS  bit;
};

struct ADCPPB3RESULT_BITS {             
    Uint16 PPBRESULT:16;                
    Uint16 SIGN:16;                     
};

union ADCPPB3RESULT_REG {
    Uint32  all;
    struct  ADCPPB3RESULT_BITS  bit;
};

struct ADCPPB4RESULT_BITS {             
    Uint16 PPBRESULT:16;                
    Uint16 SIGN:16;                     
};

union ADCPPB4RESULT_REG {
    Uint32  all;
    struct  ADCPPB4RESULT_BITS  bit;
};

struct ADC_RESULT_REGS {
    Uint16                                   ADCRESULT0;                   
    Uint16                                   ADCRESULT1;                   
    Uint16                                   ADCRESULT2;                   
    Uint16                                   ADCRESULT3;                   
    Uint16                                   ADCRESULT4;                   
    Uint16                                   ADCRESULT5;                   
    Uint16                                   ADCRESULT6;                   
    Uint16                                   ADCRESULT7;                   
    Uint16                                   ADCRESULT8;                   
    Uint16                                   ADCRESULT9;                   
    Uint16                                   ADCRESULT10;                  
    Uint16                                   ADCRESULT11;                  
    Uint16                                   ADCRESULT12;                  
    Uint16                                   ADCRESULT13;                  
    Uint16                                   ADCRESULT14;                  
    Uint16                                   ADCRESULT15;                  
    union   ADCPPB1RESULT_REG                ADCPPB1RESULT;                
    union   ADCPPB2RESULT_REG                ADCPPB2RESULT;                
    union   ADCPPB3RESULT_REG                ADCPPB3RESULT;                
    union   ADCPPB4RESULT_REG                ADCPPB4RESULT;                
};




extern volatile struct ADC_RESULT_REGS AdcaResultRegs;
extern volatile struct ADC_RESULT_REGS AdcbResultRegs;
extern volatile struct ADC_RESULT_REGS AdccResultRegs;
extern volatile struct ADC_REGS AdcaRegs;
extern volatile struct ADC_REGS AdcbRegs;
extern volatile struct ADC_REGS AdccRegs;



















































struct AES_CTRL_BITS {                  
    Uint16 OUTPUT_READY:1;              
    Uint16 INPUT_READY:1;               
    Uint16 DIRECTION:1;                 
    Uint16 KEY_SIZE:2;                  
    Uint16 MODE:1;                      
    Uint16 CTR:1;                       
    Uint16 CTR_WIDTH:2;                 
    Uint16 ICM:1;                       
    Uint16 CFB:1;                       
    Uint16 XTS:2;                       
    Uint16 F8:1;                        
    Uint16 F9:1;                        
    Uint16 CBCMAC:1;                    
    Uint16 GCM:2;                       
    Uint16 CCM:1;                       
    Uint16 CCM_L:3;                     
    Uint16 CCM_M:3;                     
    Uint16 rsvd1:4;                     
    Uint16 SAVE_CONTEXT:1;              
    Uint16 SVCTXTRDY:1;                 
    Uint16 CTXTRDY:1;                   
};

union AES_CTRL_REG {
    Uint32  all;
    struct  AES_CTRL_BITS  bit;
};

struct AES_SYSCONFIG_BITS {             
    Uint16 AUTOIDLE:1;                  
    Uint16 SOFTRESET:1;                 
    Uint16 SIDLE:2;                     
    Uint16 rsvd1:1;                     
    Uint16 DMA_REQ_DATA_IN_EN:1;        
    Uint16 DMA_REQ_DATA_OUT_EN:1;       
    Uint16 DMA_REQ_CONTEXT_IN_EN:1;     
    Uint16 DMA_REQ_CONTEXT_OUT_EN:1;    
    Uint16 MAP_CONTEXT_OUT_ON_DATA_OUT:1; 
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:3;                     
    Uint16 rsvd6:16;                    
};

union AES_SYSCONFIG_REG {
    Uint32  all;
    struct  AES_SYSCONFIG_BITS  bit;
};

struct AES_SYSSTATUS_BITS {             
    Uint16 RESETDONE:1;                 
    Uint16 rsvd1:15;                    
    Uint16 rsvd2:16;                    
};

union AES_SYSSTATUS_REG {
    Uint32  all;
    struct  AES_SYSSTATUS_BITS  bit;
};

struct AES_IRQSTATUS_BITS {             
    Uint16 CONTEXT_IN:1;                
    Uint16 DATA_IN:1;                   
    Uint16 DATA_OUT:1;                  
    Uint16 CONTEXT_OUT:1;               
    Uint16 rsvd1:12;                    
    Uint16 rsvd2:16;                    
};

union AES_IRQSTATUS_REG {
    Uint32  all;
    struct  AES_IRQSTATUS_BITS  bit;
};

struct AES_IRQENABLE_BITS {             
    Uint16 CONTEXT_IN:1;                
    Uint16 DATA_IN:1;                   
    Uint16 DATA_OUT:1;                  
    Uint16 CONTEXT_OUT:1;               
    Uint16 rsvd1:12;                    
    Uint16 rsvd2:16;                    
};

union AES_IRQENABLE_REG {
    Uint32  all;
    struct  AES_IRQENABLE_BITS  bit;
};

struct AES_DIRTY_BITS_BITS {            
    Uint16 S_ACCESS:1;                  
    Uint16 S_DIRTY:1;                   
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:12;                    
    Uint16 rsvd4:16;                    
};

union AES_DIRTY_BITS_REG {
    Uint32  all;
    struct  AES_DIRTY_BITS_BITS  bit;
};

struct AES_REGS {
    Uint32                                   AES_KEY2_6;                   
    Uint32                                   AES_KEY2_7;                   
    Uint32                                   AES_KEY2_4;                   
    Uint32                                   AES_KEY2_5;                   
    Uint32                                   AES_KEY2_2;                   
    Uint32                                   AES_KEY2_3;                   
    Uint32                                   AES_KEY2_0;                   
    Uint32                                   AES_KEY2_1;                   
    Uint32                                   AES_KEY1_6;                   
    Uint32                                   AES_KEY1_7;                   
    Uint32                                   AES_KEY1_4;                   
    Uint32                                   AES_KEY1_5;                   
    Uint32                                   AES_KEY1_2;                   
    Uint32                                   AES_KEY1_3;                   
    Uint32                                   AES_KEY1_0;                   
    Uint32                                   AES_KEY1_1;                   
    Uint32                                   AES_IV_IN_OUT_0;              
    Uint32                                   AES_IV_IN_OUT_1;              
    Uint32                                   AES_IV_IN_OUT_2;              
    Uint32                                   AES_IV_IN_OUT_3;              
    union   AES_CTRL_REG                     AES_CTRL;                     
    Uint32                                   AES_C_LENGTH_0;               
    Uint32                                   AES_C_LENGTH_1;               
    Uint32                                   AES_AUTH_LENGTH;              
    Uint32                                   AES_DATA_IN_OUT_0;            
    Uint32                                   AES_DATA_IN_OUT_1;            
    Uint32                                   AES_DATA_IN_OUT_2;            
    Uint32                                   AES_DATA_IN_OUT_3;            
    Uint32                                   AES_TAG_OUT_0;                
    Uint32                                   AES_TAG_OUT_1;                
    Uint32                                   AES_TAG_OUT_2;                
    Uint32                                   AES_TAG_OUT_3;                
    Uint32                                   AES_REV;                      
    union   AES_SYSCONFIG_REG                AES_SYSCONFIG;                
    union   AES_SYSSTATUS_REG                AES_SYSSTATUS;                
    union   AES_IRQSTATUS_REG                AES_IRQSTATUS;                
    union   AES_IRQENABLE_REG                AES_IRQENABLE;                
    union   AES_DIRTY_BITS_REG               AES_DIRTY_BITS;               
};























































struct AES_GLB_INT_FLG_BITS {           
    Uint16 INT_FLG:1;                   
    Uint16 rsvd1:15;                    
    Uint16 rsvd2:16;                    
};

union AES_GLB_INT_FLG_REG {
    Uint32  all;
    struct  AES_GLB_INT_FLG_BITS  bit;
};

struct AES_GLB_INT_CLR_BITS {           
    Uint16 INT_FLG_CLR:1;               
    Uint16 rsvd1:15;                    
    Uint16 rsvd2:16;                    
};

union AES_GLB_INT_CLR_REG {
    Uint32  all;
    struct  AES_GLB_INT_CLR_BITS  bit;
};

struct AES_SS_REGS {
    Uint16                                   rsvd1[2];                     
    union   AES_GLB_INT_FLG_REG              AES_GLB_INT_FLG;              
    union   AES_GLB_INT_CLR_REG              AES_GLB_INT_CLR;              
};























































struct CONFIGLOCK_BITS {                
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 AGPIOCTRL:1;                 
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:11;                    
    Uint16 rsvd6:16;                    
};

union CONFIGLOCK_REG {
    Uint32  all;
    struct  CONFIGLOCK_BITS  bit;
};

struct TSNSCTL_BITS {                   
    Uint16 ENABLE:1;                    
    Uint16 rsvd1:15;                    
};

union TSNSCTL_REG {
    Uint16  all;
    struct  TSNSCTL_BITS  bit;
};

struct ANAREFCTL_BITS {                 
    Uint16 ANAREFSEL:1;                 
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:5;                     
    Uint16 ANAREF2P5SEL:1;              
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:5;                     
};

union ANAREFCTL_REG {
    Uint16  all;
    struct  ANAREFCTL_BITS  bit;
};

struct VMONCTL_BITS {                   
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:6;                     
    Uint16 BORLVMONDIS:1;               
    Uint16 rsvd4:7;                     
};

union VMONCTL_REG {
    Uint16  all;
    struct  VMONCTL_BITS  bit;
};

struct CMPHPMXSEL_BITS {                
    Uint16 CMP1HPMXSEL:3;               
    Uint16 CMP2HPMXSEL:3;               
    Uint16 CMP3HPMXSEL:3;               
    Uint16 CMP4HPMXSEL:3;               
    Uint16 rsvd1:3;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:3;                     
    Uint16 rsvd4:3;                     
    Uint16 rsvd5:10;                    
};

union CMPHPMXSEL_REG {
    Uint32  all;
    struct  CMPHPMXSEL_BITS  bit;
};

struct CMPLPMXSEL_BITS {                
    Uint16 CMP1LPMXSEL:3;               
    Uint16 CMP2LPMXSEL:3;               
    Uint16 CMP3LPMXSEL:3;               
    Uint16 CMP4LPMXSEL:3;               
    Uint16 rsvd1:3;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:3;                     
    Uint16 rsvd4:3;                     
    Uint16 rsvd5:10;                    
};

union CMPLPMXSEL_REG {
    Uint32  all;
    struct  CMPLPMXSEL_BITS  bit;
};

struct CMPHNMXSEL_BITS {                
    Uint16 CMP1HNMXSEL:1;               
    Uint16 CMP2HNMXSEL:1;               
    Uint16 CMP3HNMXSEL:1;               
    Uint16 CMP4HNMXSEL:1;               
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:9;                     
};

union CMPHNMXSEL_REG {
    Uint16  all;
    struct  CMPHNMXSEL_BITS  bit;
};

struct CMPLNMXSEL_BITS {                
    Uint16 CMP1LNMXSEL:1;               
    Uint16 CMP2LNMXSEL:1;               
    Uint16 CMP3LNMXSEL:1;               
    Uint16 CMP4LNMXSEL:1;               
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:9;                     
};

union CMPLNMXSEL_REG {
    Uint16  all;
    struct  CMPLNMXSEL_BITS  bit;
};

struct ADCDACLOOPBACK_BITS {            
    Uint16 ENLB2ADCA:1;                 
    Uint16 ENLB2ADCB:1;                 
    Uint16 ENLB2ADCC:1;                 
    Uint16 rsvd1:13;                    
    Uint16 KEY:16;                      
};

union ADCDACLOOPBACK_REG {
    Uint32  all;
    struct  ADCDACLOOPBACK_BITS  bit;
};

struct LOCK_BITS {                      
    Uint16 TSNSCTL:1;                   
    Uint16 ANAREFCTL:1;                 
    Uint16 VMONCTL:1;                   
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 CMPHPMXSEL:1;                
    Uint16 CMPLPMXSEL:1;                
    Uint16 CMPHNMXSEL:1;                
    Uint16 CMPLNMXSEL:1;                
    Uint16 VREGCTL:1;                   
    Uint16 rsvd3:6;                     
    Uint16 rsvd4:16;                    
};

union LOCK_REG {
    Uint32  all;
    struct  LOCK_BITS  bit;
};

struct AGPIOCTRLA_BITS {                
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:1;                     
    Uint16 rsvd8:1;                     
    Uint16 rsvd9:1;                     
    Uint16 rsvd10:1;                    
    Uint16 rsvd11:1;                    
    Uint16 rsvd12:1;                    
    Uint16 rsvd13:1;                    
    Uint16 rsvd14:1;                    
    Uint16 rsvd15:1;                    
    Uint16 rsvd16:1;                    
    Uint16 rsvd17:1;                    
    Uint16 rsvd18:1;                    
    Uint16 rsvd19:1;                    
    Uint16 rsvd20:1;                    
    Uint16 GPIO20:1;                    
    Uint16 GPIO21:1;                    
    Uint16 rsvd21:1;                    
    Uint16 rsvd22:1;                    
    Uint16 rsvd23:1;                    
    Uint16 rsvd24:1;                    
    Uint16 rsvd25:1;                    
    Uint16 rsvd26:1;                    
    Uint16 rsvd27:1;                    
    Uint16 rsvd28:1;                    
    Uint16 rsvd29:1;                    
    Uint16 rsvd30:1;                    
};

union AGPIOCTRLA_REG {
    Uint32  all;
    struct  AGPIOCTRLA_BITS  bit;
};

struct ANALOG_SUBSYS_REGS {
    Uint16                                   rsvd1[94];                    
    union   CONFIGLOCK_REG                   CONFIGLOCK;                   
    union   TSNSCTL_REG                      TSNSCTL;                      
    Uint16                                   rsvd2[7];                     
    union   ANAREFCTL_REG                    ANAREFCTL;                    
    Uint16                                   rsvd3[7];                     
    union   VMONCTL_REG                      VMONCTL;                      
    Uint16                                   rsvd4[17];                    
    union   CMPHPMXSEL_REG                   CMPHPMXSEL;                   
    union   CMPLPMXSEL_REG                   CMPLPMXSEL;                   
    union   CMPHNMXSEL_REG                   CMPHNMXSEL;                   
    union   CMPLNMXSEL_REG                   CMPLNMXSEL;                   
    union   ADCDACLOOPBACK_REG               ADCDACLOOPBACK;               
    Uint16                                   rsvd5[4];                     
    union   LOCK_REG                         LOCK;                         
    Uint16                                   rsvd6[114];                   
    union   AGPIOCTRLA_REG                   AGPIOCTRLA;                   
};




extern volatile struct ANALOG_SUBSYS_REGS AnalogSubsysRegs;



















































struct BGCRC_EN_BITS {                  
    Uint16 START:4;                     
    Uint16 rsvd1:12;                    
    Uint16 rsvd2:15;                    
    Uint16 RUN_STS:1;                   
};

union BGCRC_EN_REG {
    Uint32  all;
    struct  BGCRC_EN_BITS  bit;
};

struct BGCRC_CTRL1_BITS {               
    Uint16 rsvd1:4;                     
    Uint16 FREE_SOFT:1;                 
    Uint16 rsvd2:11;                    
    Uint16 NMIDIS:4;                    
    Uint16 rsvd3:12;                    
};

union BGCRC_CTRL1_REG {
    Uint32  all;
    struct  BGCRC_CTRL1_BITS  bit;
};

struct BGCRC_CTRL2_BITS {               
    Uint16 BLOCK_SIZE:10;               
    Uint16 rsvd1:2;                     
    Uint16 TEST_HALT:4;                 
    Uint16 SCRUB_MODE:4;                
    Uint16 rsvd2:12;                    
};

union BGCRC_CTRL2_REG {
    Uint32  all;
    struct  BGCRC_CTRL2_BITS  bit;
};

struct BGCRC_WD_CFG_BITS {              
    Uint16 WDDIS:4;                     
    Uint16 rsvd1:12;                    
    Uint16 rsvd2:16;                    
};

union BGCRC_WD_CFG_REG {
    Uint32  all;
    struct  BGCRC_WD_CFG_BITS  bit;
};

struct BGCRC_NMIFLG_BITS {              
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 CRC_FAIL:1;                  
    Uint16 UNCORRECTABLE_ERR:1;         
    Uint16 CORRECTABLE_ERR:1;           
    Uint16 WD_UNDERFLOW:1;              
    Uint16 WD_OVERFLOW:1;               
    Uint16 rsvd3:9;                     
    Uint16 rsvd4:16;                    
};

union BGCRC_NMIFLG_REG {
    Uint32  all;
    struct  BGCRC_NMIFLG_BITS  bit;
};

struct BGCRC_NMICLR_BITS {              
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 CRC_FAIL:1;                  
    Uint16 UNCORRECTABLE_ERR:1;         
    Uint16 CORRECTABLE_ERR:1;           
    Uint16 WD_UNDERFLOW:1;              
    Uint16 WD_OVERFLOW:1;               
    Uint16 rsvd3:9;                     
    Uint16 rsvd4:16;                    
};

union BGCRC_NMICLR_REG {
    Uint32  all;
    struct  BGCRC_NMICLR_BITS  bit;
};

struct BGCRC_NMIFRC_BITS {              
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 CRC_FAIL:1;                  
    Uint16 UNCORRECTABLE_ERR:1;         
    Uint16 CORRECTABLE_ERR:1;           
    Uint16 WD_UNDERFLOW:1;              
    Uint16 WD_OVERFLOW:1;               
    Uint16 rsvd3:9;                     
    Uint16 rsvd4:16;                    
};

union BGCRC_NMIFRC_REG {
    Uint32  all;
    struct  BGCRC_NMIFRC_BITS  bit;
};

struct BGCRC_INTEN_BITS {               
    Uint16 rsvd1:1;                     
    Uint16 TEST_DONE:1;                 
    Uint16 CRC_FAIL:1;                  
    Uint16 UNCORRECTABLE_ERR:1;         
    Uint16 CORRECTABLE_ERR:1;           
    Uint16 WD_UNDERFLOW:1;              
    Uint16 WD_OVERFLOW:1;               
    Uint16 rsvd2:9;                     
    Uint16 rsvd3:16;                    
};

union BGCRC_INTEN_REG {
    Uint32  all;
    struct  BGCRC_INTEN_BITS  bit;
};

struct BGCRC_INTFLG_BITS {              
    Uint16 INT:1;                       
    Uint16 TEST_DONE:1;                 
    Uint16 CRC_FAIL:1;                  
    Uint16 UNCORRECTABLE_ERR:1;         
    Uint16 CORRECTABLE_ERR:1;           
    Uint16 WD_UNDERFLOW:1;              
    Uint16 WD_OVERFLOW:1;               
    Uint16 rsvd1:9;                     
    Uint16 rsvd2:16;                    
};

union BGCRC_INTFLG_REG {
    Uint32  all;
    struct  BGCRC_INTFLG_BITS  bit;
};

struct BGCRC_INTCLR_BITS {              
    Uint16 INT:1;                       
    Uint16 TEST_DONE:1;                 
    Uint16 CRC_FAIL:1;                  
    Uint16 UNCORRECTABLE_ERR:1;         
    Uint16 CORRECTABLE_ERR:1;           
    Uint16 WD_UNDERFLOW:1;              
    Uint16 WD_OVERFLOW:1;               
    Uint16 rsvd1:9;                     
    Uint16 rsvd2:16;                    
};

union BGCRC_INTCLR_REG {
    Uint32  all;
    struct  BGCRC_INTCLR_BITS  bit;
};

struct BGCRC_INTFRC_BITS {              
    Uint16 rsvd1:1;                     
    Uint16 TEST_DONE:1;                 
    Uint16 CRC_FAIL:1;                  
    Uint16 UNCORRECTABLE_ERR:1;         
    Uint16 CORRECTABLE_ERR:1;           
    Uint16 WD_UNDERFLOW:1;              
    Uint16 WD_OVERFLOW:1;               
    Uint16 rsvd2:9;                     
    Uint16 rsvd3:16;                    
};

union BGCRC_INTFRC_REG {
    Uint32  all;
    struct  BGCRC_INTFRC_BITS  bit;
};

struct BGCRC_LOCK_BITS {                
    Uint16 BGCRC_EN:1;                  
    Uint16 BGCRC_CTRL1:1;               
    Uint16 BGCRC_CTRL2:1;               
    Uint16 BGCRC_START_ADDR:1;          
    Uint16 BGCRC_SEED:1;                
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 BGCRC_GOLDEN:1;              
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:1;                     
    Uint16 rsvd8:1;                     
    Uint16 BGCRC_WD_CFG:1;              
    Uint16 BGCRC_WD_MIN:1;              
    Uint16 BGCRC_WD_MAX:1;              
    Uint16 rsvd9:1;                     
    Uint16 rsvd10:1;                    
    Uint16 rsvd11:1;                    
    Uint16 rsvd12:1;                    
    Uint16 rsvd13:1;                    
    Uint16 rsvd14:1;                    
    Uint16 BGCRC_NMIFRC:1;              
    Uint16 rsvd15:1;                    
    Uint16 rsvd16:1;                    
    Uint16 BGCRC_INTEN:1;               
    Uint16 rsvd17:1;                    
    Uint16 rsvd18:1;                    
    Uint16 BGCRC_INTFRC:1;              
    Uint16 rsvd19:1;                    
    Uint16 rsvd20:1;                    
};

union BGCRC_LOCK_REG {
    Uint32  all;
    struct  BGCRC_LOCK_BITS  bit;
};

struct BGCRC_COMMIT_BITS {              
    Uint16 BGCRC_EN:1;                  
    Uint16 BGCRC_CTRL1:1;               
    Uint16 BGCRC_CTRL2:1;               
    Uint16 BGCRC_START_ADDR:1;          
    Uint16 BGCRC_SEED:1;                
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 BGCRC_GOLDEN:1;              
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:1;                     
    Uint16 rsvd8:1;                     
    Uint16 BGCRC_WD_CFG:1;              
    Uint16 BGCRC_WD_MIN:1;              
    Uint16 BGCRC_WD_MAX:1;              
    Uint16 rsvd9:1;                     
    Uint16 rsvd10:1;                    
    Uint16 rsvd11:1;                    
    Uint16 rsvd12:1;                    
    Uint16 rsvd13:1;                    
    Uint16 rsvd14:1;                    
    Uint16 BGCRC_NMIFRC:1;              
    Uint16 rsvd15:1;                    
    Uint16 rsvd16:1;                    
    Uint16 BGCRC_INTEN:1;               
    Uint16 rsvd17:1;                    
    Uint16 rsvd18:1;                    
    Uint16 BGCRC_INTFRC:1;              
    Uint16 rsvd19:1;                    
    Uint16 rsvd20:1;                    
};

union BGCRC_COMMIT_REG {
    Uint32  all;
    struct  BGCRC_COMMIT_BITS  bit;
};

struct BGCRC_REGS {
    union   BGCRC_EN_REG                     BGCRC_EN;                     
    union   BGCRC_CTRL1_REG                  BGCRC_CTRL1;                  
    union   BGCRC_CTRL2_REG                  BGCRC_CTRL2;                  
    Uint32                                   BGCRC_START_ADDR;             
    Uint32                                   BGCRC_SEED;                   
    Uint16                                   rsvd1[4];                     
    Uint32                                   BGCRC_GOLDEN;                 
    Uint32                                   BGCRC_RESULT;                 
    Uint32                                   BGCRC_CURR_ADDR;              
    Uint16                                   rsvd2[8];                     
    union   BGCRC_WD_CFG_REG                 BGCRC_WD_CFG;                 
    Uint32                                   BGCRC_WD_MIN;                 
    Uint32                                   BGCRC_WD_MAX;                 
    Uint32                                   BGCRC_WD_CNT;                 
    Uint16                                   rsvd3[6];                     
    union   BGCRC_NMIFLG_REG                 BGCRC_NMIFLG;                 
    union   BGCRC_NMICLR_REG                 BGCRC_NMICLR;                 
    union   BGCRC_NMIFRC_REG                 BGCRC_NMIFRC;                 
    Uint16                                   rsvd4[4];                     
    union   BGCRC_INTEN_REG                  BGCRC_INTEN;                  
    union   BGCRC_INTFLG_REG                 BGCRC_INTFLG;                 
    union   BGCRC_INTCLR_REG                 BGCRC_INTCLR;                 
    union   BGCRC_INTFRC_REG                 BGCRC_INTFRC;                 
    union   BGCRC_LOCK_REG                   BGCRC_LOCK;                   
    union   BGCRC_COMMIT_REG                 BGCRC_COMMIT;                 
};




extern volatile struct BGCRC_REGS BgcrcCpuRegs;
extern volatile struct BGCRC_REGS BgcrcCla1Regs;



















































struct MCTL_BITS {                      
    Uint16 HARDRESET:1;                 
    Uint16 SOFTRESET:1;                 
    Uint16 IACKE:1;                     
    Uint16 rsvd1:13;                    
};

union MCTL_REG {
    Uint16  all;
    struct  MCTL_BITS  bit;
};

struct SOFTINTEN_BITS {                 
    Uint16 TASK1:1;                     
    Uint16 TASK2:1;                     
    Uint16 TASK3:1;                     
    Uint16 TASK4:1;                     
    Uint16 TASK5:1;                     
    Uint16 TASK6:1;                     
    Uint16 TASK7:1;                     
    Uint16 TASK8:1;                     
    Uint16 rsvd1:8;                     
};

union SOFTINTEN_REG {
    Uint16  all;
    struct  SOFTINTEN_BITS  bit;
};

struct _MSTSBGRND_BITS {                
    Uint16 RUN:1;                       
    Uint16 _BGINTM:1;                   
    Uint16 BGOVF:1;                     
    Uint16 rsvd1:13;                    
};

union _MSTSBGRND_REG {
    Uint16  all;
    struct  _MSTSBGRND_BITS  bit;
};

struct _MCTLBGRND_BITS {                
    Uint16 BGSTART:1;                   
    Uint16 TRIGEN:1;                    
    Uint16 rsvd1:13;                    
    Uint16 BGEN:1;                      
};

union _MCTLBGRND_REG {
    Uint16  all;
    struct  _MCTLBGRND_BITS  bit;
};

struct MIFR_BITS {                      
    Uint16 INT1:1;                      
    Uint16 INT2:1;                      
    Uint16 INT3:1;                      
    Uint16 INT4:1;                      
    Uint16 INT5:1;                      
    Uint16 INT6:1;                      
    Uint16 INT7:1;                      
    Uint16 INT8:1;                      
    Uint16 rsvd1:8;                     
};

union MIFR_REG {
    Uint16  all;
    struct  MIFR_BITS  bit;
};

struct MIOVF_BITS {                     
    Uint16 INT1:1;                      
    Uint16 INT2:1;                      
    Uint16 INT3:1;                      
    Uint16 INT4:1;                      
    Uint16 INT5:1;                      
    Uint16 INT6:1;                      
    Uint16 INT7:1;                      
    Uint16 INT8:1;                      
    Uint16 rsvd1:8;                     
};

union MIOVF_REG {
    Uint16  all;
    struct  MIOVF_BITS  bit;
};

struct MIFRC_BITS {                     
    Uint16 INT1:1;                      
    Uint16 INT2:1;                      
    Uint16 INT3:1;                      
    Uint16 INT4:1;                      
    Uint16 INT5:1;                      
    Uint16 INT6:1;                      
    Uint16 INT7:1;                      
    Uint16 INT8:1;                      
    Uint16 rsvd1:8;                     
};

union MIFRC_REG {
    Uint16  all;
    struct  MIFRC_BITS  bit;
};

struct MICLR_BITS {                     
    Uint16 INT1:1;                      
    Uint16 INT2:1;                      
    Uint16 INT3:1;                      
    Uint16 INT4:1;                      
    Uint16 INT5:1;                      
    Uint16 INT6:1;                      
    Uint16 INT7:1;                      
    Uint16 INT8:1;                      
    Uint16 rsvd1:8;                     
};

union MICLR_REG {
    Uint16  all;
    struct  MICLR_BITS  bit;
};

struct MICLROVF_BITS {                  
    Uint16 INT1:1;                      
    Uint16 INT2:1;                      
    Uint16 INT3:1;                      
    Uint16 INT4:1;                      
    Uint16 INT5:1;                      
    Uint16 INT6:1;                      
    Uint16 INT7:1;                      
    Uint16 INT8:1;                      
    Uint16 rsvd1:8;                     
};

union MICLROVF_REG {
    Uint16  all;
    struct  MICLROVF_BITS  bit;
};

struct MIER_BITS {                      
    Uint16 INT1:1;                      
    Uint16 INT2:1;                      
    Uint16 INT3:1;                      
    Uint16 INT4:1;                      
    Uint16 INT5:1;                      
    Uint16 INT6:1;                      
    Uint16 INT7:1;                      
    Uint16 INT8:1;                      
    Uint16 rsvd1:8;                     
};

union MIER_REG {
    Uint16  all;
    struct  MIER_BITS  bit;
};

struct MIRUN_BITS {                     
    Uint16 INT1:1;                      
    Uint16 INT2:1;                      
    Uint16 INT3:1;                      
    Uint16 INT4:1;                      
    Uint16 INT5:1;                      
    Uint16 INT6:1;                      
    Uint16 INT7:1;                      
    Uint16 INT8:1;                      
    Uint16 rsvd1:8;                     
};

union MIRUN_REG {
    Uint16  all;
    struct  MIRUN_BITS  bit;
};

struct _MSTF_BITS {                     
    Uint16 LVF:1;                       
    Uint16 LUF:1;                       
    Uint16 NF:1;                        
    Uint16 ZF:1;                        
    Uint16 rsvd1:2;                     
    Uint16 TF:1;                        
    Uint16 rsvd2:2;                     
    Uint16 RNDF32:1;                    
    Uint16 rsvd3:1;                     
    Uint16 MEALLOW:1;                   
    Uint32 _RPC:16;                     
    Uint16 rsvd4:4;                     
};

union _MSTF_REG {
    Uint32  all;
    struct  _MSTF_BITS  bit;
};

struct _MPSACTL_BITS {                  
    Uint16 MPABSTART:1;                 
    Uint16 MPABCYC:1;                   
    Uint16 MDWDBSTART:1;                
    Uint16 MDWDBCYC:1;                  
    Uint16 MPSA1CLEAR:1;                
    Uint16 MPSA2CLEAR:1;                
    Uint16 MPSA2CFG:2;                  
    Uint16 rsvd1:8;                     
};

union _MPSACTL_REG {
    Uint16  all;
    struct  _MPSACTL_BITS  bit;
};

union MR_REG {
    Uint32              i32;
    float               f32;
};

struct CLA_REGS {
    Uint16                                   MVECT1;                       
    Uint16                                   MVECT2;                       
    Uint16                                   MVECT3;                       
    Uint16                                   MVECT4;                       
    Uint16                                   MVECT5;                       
    Uint16                                   MVECT6;                       
    Uint16                                   MVECT7;                       
    Uint16                                   MVECT8;                       
    Uint16                                   rsvd1[8];                     
    union   MCTL_REG                         MCTL;                         
    Uint16                                   rsvd2[10];                    
    Uint16                                   _MVECTBGRNDACTIVE;            
    union   SOFTINTEN_REG                    SOFTINTEN;                    
    union   _MSTSBGRND_REG                   _MSTSBGRND;                   
    union   _MCTLBGRND_REG                   _MCTLBGRND;                   
    Uint16                                   _MVECTBGRND;                  
    union   MIFR_REG                         MIFR;                         
    union   MIOVF_REG                        MIOVF;                        
    union   MIFRC_REG                        MIFRC;                        
    union   MICLR_REG                        MICLR;                        
    union   MICLROVF_REG                     MICLROVF;                     
    union   MIER_REG                         MIER;                         
    union   MIRUN_REG                        MIRUN;                        
    Uint16                                   rsvd3;                        
    Uint16                                   _MPC;                         
    Uint16                                   rsvd4;                        
    Uint16                                   _MAR0;                        
    Uint16                                   _MAR1;                        
    Uint16                                   rsvd5[2];                     
    union   _MSTF_REG                        _MSTF;                        
    union   MR_REG                           _MR0;                         
    Uint16                                   rsvd6[2];                     
    union   MR_REG                           _MR1;                         
    Uint16                                   rsvd7[2];                     
    union   MR_REG                           _MR2;                         
    Uint16                                   rsvd8[2];                     
    union   MR_REG                           _MR3;                         
    Uint16                                   rsvd9[4];                     
    union   _MPSACTL_REG                     _MPSACTL;                     
    Uint16                                   rsvd10;                       
    Uint32                                   _MPSA1;                       
    Uint32                                   _MPSA2;                       
};

struct SOFTINTFRC_BITS {                
    Uint16 TASK1:1;                     
    Uint16 TASK2:1;                     
    Uint16 TASK3:1;                     
    Uint16 TASK4:1;                     
    Uint16 TASK5:1;                     
    Uint16 TASK6:1;                     
    Uint16 TASK7:1;                     
    Uint16 TASK8:1;                     
    Uint16 rsvd1:8;                     
};

union SOFTINTFRC_REG {
    Uint16  all;
    struct  SOFTINTFRC_BITS  bit;
};

struct CLA_ONLY_REGS {
    Uint16                                   rsvd1[128];                   
    Uint16                                   _MVECTBGRNDACTIVE;            
    Uint16                                   rsvd2[63];                    
    union   _MPSACTL_REG                     _MPSACTL;                     
    Uint16                                   rsvd3;                        
    Uint32                                   _MPSA1;                       
    Uint32                                   _MPSA2;                       
    Uint16                                   rsvd4[26];                    
    union   SOFTINTEN_REG                    SOFTINTEN;                    
    Uint16                                   rsvd5;                        
    union   SOFTINTFRC_REG                   SOFTINTFRC;                   
};

struct CLA_SOFTINT_REGS {
    union   SOFTINTEN_REG                    SOFTINTEN;                    
    Uint16                                   rsvd1;                        
    union   SOFTINTFRC_REG                   SOFTINTFRC;                   
};




extern volatile struct CLA_REGS Cla1Regs;



















































struct CLB_COUNT_RESET_BITS {           
    Uint16 SEL_0:5;                     
    Uint16 SEL_1:5;                     
    Uint16 SEL_2:5;                     
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:16;                    
};

union CLB_COUNT_RESET_REG {
    Uint32  all;
    struct  CLB_COUNT_RESET_BITS  bit;
};

struct CLB_COUNT_MODE_1_BITS {          
    Uint16 SEL_0:5;                     
    Uint16 SEL_1:5;                     
    Uint16 SEL_2:5;                     
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:16;                    
};

union CLB_COUNT_MODE_1_REG {
    Uint32  all;
    struct  CLB_COUNT_MODE_1_BITS  bit;
};

struct CLB_COUNT_MODE_0_BITS {          
    Uint16 SEL_0:5;                     
    Uint16 SEL_1:5;                     
    Uint16 SEL_2:5;                     
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:16;                    
};

union CLB_COUNT_MODE_0_REG {
    Uint32  all;
    struct  CLB_COUNT_MODE_0_BITS  bit;
};

struct CLB_COUNT_EVENT_BITS {           
    Uint16 SEL_0:5;                     
    Uint16 SEL_1:5;                     
    Uint16 SEL_2:5;                     
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:16;                    
};

union CLB_COUNT_EVENT_REG {
    Uint32  all;
    struct  CLB_COUNT_EVENT_BITS  bit;
};

struct CLB_FSM_EXTRA_IN0_BITS {         
    Uint16 SEL_0:5;                     
    Uint16 SEL_1:5;                     
    Uint16 SEL_2:5;                     
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:16;                    
};

union CLB_FSM_EXTRA_IN0_REG {
    Uint32  all;
    struct  CLB_FSM_EXTRA_IN0_BITS  bit;
};

struct CLB_FSM_EXTERNAL_IN0_BITS {      
    Uint16 SEL_0:5;                     
    Uint16 SEL_1:5;                     
    Uint16 SEL_2:5;                     
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:16;                    
};

union CLB_FSM_EXTERNAL_IN0_REG {
    Uint32  all;
    struct  CLB_FSM_EXTERNAL_IN0_BITS  bit;
};

struct CLB_FSM_EXTERNAL_IN1_BITS {      
    Uint16 SEL_0:5;                     
    Uint16 SEL_1:5;                     
    Uint16 SEL_2:5;                     
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:16;                    
};

union CLB_FSM_EXTERNAL_IN1_REG {
    Uint32  all;
    struct  CLB_FSM_EXTERNAL_IN1_BITS  bit;
};

struct CLB_FSM_EXTRA_IN1_BITS {         
    Uint16 SEL_0:5;                     
    Uint16 SEL_1:5;                     
    Uint16 SEL_2:5;                     
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:16;                    
};

union CLB_FSM_EXTRA_IN1_REG {
    Uint32  all;
    struct  CLB_FSM_EXTRA_IN1_BITS  bit;
};

struct CLB_LUT4_IN0_BITS {              
    Uint16 SEL_0:5;                     
    Uint16 SEL_1:5;                     
    Uint16 SEL_2:5;                     
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:16;                    
};

union CLB_LUT4_IN0_REG {
    Uint32  all;
    struct  CLB_LUT4_IN0_BITS  bit;
};

struct CLB_LUT4_IN1_BITS {              
    Uint16 SEL_0:5;                     
    Uint16 SEL_1:5;                     
    Uint16 SEL_2:5;                     
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:16;                    
};

union CLB_LUT4_IN1_REG {
    Uint32  all;
    struct  CLB_LUT4_IN1_BITS  bit;
};

struct CLB_LUT4_IN2_BITS {              
    Uint16 SEL_0:5;                     
    Uint16 SEL_1:5;                     
    Uint16 SEL_2:5;                     
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:16;                    
};

union CLB_LUT4_IN2_REG {
    Uint32  all;
    struct  CLB_LUT4_IN2_BITS  bit;
};

struct CLB_LUT4_IN3_BITS {              
    Uint16 SEL_0:5;                     
    Uint16 SEL_1:5;                     
    Uint16 SEL_2:5;                     
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:16;                    
};

union CLB_LUT4_IN3_REG {
    Uint32  all;
    struct  CLB_LUT4_IN3_BITS  bit;
};

struct CLB_FSM_LUT_FN1_0_BITS {         
    Uint16 FN0:16;                      
    Uint16 FN1:16;                      
};

union CLB_FSM_LUT_FN1_0_REG {
    Uint32  all;
    struct  CLB_FSM_LUT_FN1_0_BITS  bit;
};

struct CLB_FSM_LUT_FN2_BITS {           
    Uint16 FN1:16;                      
    Uint16 rsvd1:16;                    
};

union CLB_FSM_LUT_FN2_REG {
    Uint32  all;
    struct  CLB_FSM_LUT_FN2_BITS  bit;
};

struct CLB_LUT4_FN1_0_BITS {            
    Uint16 FN0:16;                      
    Uint16 FN1:16;                      
};

union CLB_LUT4_FN1_0_REG {
    Uint32  all;
    struct  CLB_LUT4_FN1_0_BITS  bit;
};

struct CLB_LUT4_FN2_BITS {              
    Uint16 FN1:16;                      
    Uint16 rsvd1:16;                    
};

union CLB_LUT4_FN2_REG {
    Uint32  all;
    struct  CLB_LUT4_FN2_BITS  bit;
};

struct CLB_FSM_NEXT_STATE_0_BITS {      
    Uint16 S0:16;                       
    Uint16 S1:16;                       
};

union CLB_FSM_NEXT_STATE_0_REG {
    Uint32  all;
    struct  CLB_FSM_NEXT_STATE_0_BITS  bit;
};

struct CLB_FSM_NEXT_STATE_1_BITS {      
    Uint16 S0:16;                       
    Uint16 S1:16;                       
};

union CLB_FSM_NEXT_STATE_1_REG {
    Uint32  all;
    struct  CLB_FSM_NEXT_STATE_1_BITS  bit;
};

struct CLB_FSM_NEXT_STATE_2_BITS {      
    Uint16 S0:16;                       
    Uint16 S1:16;                       
};

union CLB_FSM_NEXT_STATE_2_REG {
    Uint32  all;
    struct  CLB_FSM_NEXT_STATE_2_BITS  bit;
};

struct CLB_MISC_CONTROL_BITS {          
    Uint16 COUNT_ADD_SHIFT_0:1;         
    Uint16 COUNT_DIR_0:1;               
    Uint16 COUNT_EVENT_CTRL_0:1;        
    Uint16 COUNT_ADD_SHIFT_1:1;         
    Uint16 COUNT_DIR_1:1;               
    Uint16 COUNT_EVENT_CTRL_1:1;        
    Uint16 COUNT_ADD_SHIFT_2:1;         
    Uint16 COUNT_DIR_2:1;               
    Uint16 COUNT_EVENT_CTRL_2:1;        
    Uint16 COUNT_SERIALIZER_0:1;        
    Uint16 COUNT_SERIALIZER_1:1;        
    Uint16 COUNT_SERIALIZER_2:1;        
    Uint16 FSM_EXTRA_SEL0_0:1;          
    Uint16 FSM_EXTRA_SEL1_0:1;          
    Uint16 FSM_EXTRA_SEL0_1:1;          
    Uint16 FSM_EXTRA_SEL1_1:1;          
    Uint16 FSM_EXTRA_SEL0_2:1;          
    Uint16 FSM_EXTRA_SEL1_2:1;          
    Uint16 COUNT0_MATCH1_TAP_EN:1;      
    Uint16 COUNT1_MATCH1_TAP_EN:1;      
    Uint16 COUNT2_MATCH1_TAP_EN:1;      
    Uint16 COUNT0_MATCH2_TAP_EN:1;      
    Uint16 COUNT1_MATCH2_TAP_EN:1;      
    Uint16 COUNT2_MATCH2_TAP_EN:1;      
    Uint16 COUNT0_LFSR_EN:1;            
    Uint16 COUNT1_LFSR_EN:1;            
    Uint16 COUNT2_LFSR_EN:1;            
    Uint16 rsvd1:5;                     
};

union CLB_MISC_CONTROL_REG {
    Uint32  all;
    struct  CLB_MISC_CONTROL_BITS  bit;
};

struct CLB_OUTPUT_LUT_0_BITS {          
    Uint16 IN0:5;                       
    Uint16 IN1:5;                       
    Uint16 IN2:5;                       
    Uint32 FN:8;                        
    Uint16 rsvd1:9;                     
};

union CLB_OUTPUT_LUT_0_REG {
    Uint32  all;
    struct  CLB_OUTPUT_LUT_0_BITS  bit;
};

struct CLB_OUTPUT_LUT_1_BITS {          
    Uint16 IN0:5;                       
    Uint16 IN1:5;                       
    Uint16 IN2:5;                       
    Uint32 FN:8;                        
    Uint16 rsvd1:9;                     
};

union CLB_OUTPUT_LUT_1_REG {
    Uint32  all;
    struct  CLB_OUTPUT_LUT_1_BITS  bit;
};

struct CLB_OUTPUT_LUT_2_BITS {          
    Uint16 IN0:5;                       
    Uint16 IN1:5;                       
    Uint16 IN2:5;                       
    Uint32 FN:8;                        
    Uint16 rsvd1:9;                     
};

union CLB_OUTPUT_LUT_2_REG {
    Uint32  all;
    struct  CLB_OUTPUT_LUT_2_BITS  bit;
};

struct CLB_OUTPUT_LUT_3_BITS {          
    Uint16 IN0:5;                       
    Uint16 IN1:5;                       
    Uint16 IN2:5;                       
    Uint32 FN:8;                        
    Uint16 rsvd1:9;                     
};

union CLB_OUTPUT_LUT_3_REG {
    Uint32  all;
    struct  CLB_OUTPUT_LUT_3_BITS  bit;
};

struct CLB_OUTPUT_LUT_4_BITS {          
    Uint16 IN0:5;                       
    Uint16 IN1:5;                       
    Uint16 IN2:5;                       
    Uint32 FN:8;                        
    Uint16 rsvd1:9;                     
};

union CLB_OUTPUT_LUT_4_REG {
    Uint32  all;
    struct  CLB_OUTPUT_LUT_4_BITS  bit;
};

struct CLB_OUTPUT_LUT_5_BITS {          
    Uint16 IN0:5;                       
    Uint16 IN1:5;                       
    Uint16 IN2:5;                       
    Uint32 FN:8;                        
    Uint16 rsvd1:9;                     
};

union CLB_OUTPUT_LUT_5_REG {
    Uint32  all;
    struct  CLB_OUTPUT_LUT_5_BITS  bit;
};

struct CLB_OUTPUT_LUT_6_BITS {          
    Uint16 IN0:5;                       
    Uint16 IN1:5;                       
    Uint16 IN2:5;                       
    Uint32 FN:8;                        
    Uint16 rsvd1:9;                     
};

union CLB_OUTPUT_LUT_6_REG {
    Uint32  all;
    struct  CLB_OUTPUT_LUT_6_BITS  bit;
};

struct CLB_OUTPUT_LUT_7_BITS {          
    Uint16 IN0:5;                       
    Uint16 IN1:5;                       
    Uint16 IN2:5;                       
    Uint32 FN:8;                        
    Uint16 rsvd1:9;                     
};

union CLB_OUTPUT_LUT_7_REG {
    Uint32  all;
    struct  CLB_OUTPUT_LUT_7_BITS  bit;
};

struct CLB_HLC_EVENT_SEL_BITS {         
    Uint16 EVENT0_SEL:5;                
    Uint16 EVENT1_SEL:5;                
    Uint16 EVENT2_SEL:5;                
    Uint32 EVENT3_SEL:5;                
    Uint16 ALT_EVENT0_SEL:1;            
    Uint16 ALT_EVENT1_SEL:1;            
    Uint16 ALT_EVENT2_SEL:1;            
    Uint16 ALT_EVENT3_SEL:1;            
    Uint16 rsvd1:8;                     
};

union CLB_HLC_EVENT_SEL_REG {
    Uint32  all;
    struct  CLB_HLC_EVENT_SEL_BITS  bit;
};

struct CLB_COUNT_MATCH_TAP_SEL_BITS {   
    Uint16 COUNT0_MATCH1:5;             
    Uint16 COUNT1_MATCH1:5;             
    Uint16 COUNT2_MATCH1:5;             
    Uint16 rsvd1:1;                     
    Uint16 COUNT0_MATCH2:5;             
    Uint16 COUNT1_MATCH2:5;             
    Uint16 COUNT2_MATCH2:5;             
    Uint16 rsvd2:1;                     
};

union CLB_COUNT_MATCH_TAP_SEL_REG {
    Uint32  all;
    struct  CLB_COUNT_MATCH_TAP_SEL_BITS  bit;
};

struct CLB_OUTPUT_COND_CTRL_0_BITS {    
    Uint16 LEVEL_1_SEL:1;               
    Uint16 LEVEL_2_SEL:2;               
    Uint16 LEVEL_3_SEL:2;               
    Uint16 SEL_GATING_CTRL:3;           
    Uint16 SEL_RELEASE_CTRL:3;          
    Uint16 HW_GATING_CTRL_SEL:1;        
    Uint16 HW_RLS_CTRL_SEL:1;           
    Uint16 SEL_RAW_IN:1;                
    Uint16 ASYNC_COND_EN:1;             
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:16;                    
};

union CLB_OUTPUT_COND_CTRL_0_REG {
    Uint32  all;
    struct  CLB_OUTPUT_COND_CTRL_0_BITS  bit;
};

struct CLB_OUTPUT_COND_CTRL_1_BITS {    
    Uint16 LEVEL_1_SEL:1;               
    Uint16 LEVEL_2_SEL:2;               
    Uint16 LEVEL_3_SEL:2;               
    Uint16 SEL_GATING_CTRL:3;           
    Uint16 SEL_RELEASE_CTRL:3;          
    Uint16 HW_GATING_CTRL_SEL:1;        
    Uint16 HW_RLS_CTRL_SEL:1;           
    Uint16 SEL_RAW_IN:1;                
    Uint16 ASYNC_COND_EN:1;             
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:16;                    
};

union CLB_OUTPUT_COND_CTRL_1_REG {
    Uint32  all;
    struct  CLB_OUTPUT_COND_CTRL_1_BITS  bit;
};

struct CLB_OUTPUT_COND_CTRL_2_BITS {    
    Uint16 LEVEL_1_SEL:1;               
    Uint16 LEVEL_2_SEL:2;               
    Uint16 LEVEL_3_SEL:2;               
    Uint16 SEL_GATING_CTRL:3;           
    Uint16 SEL_RELEASE_CTRL:3;          
    Uint16 HW_GATING_CTRL_SEL:1;        
    Uint16 HW_RLS_CTRL_SEL:1;           
    Uint16 SEL_RAW_IN:1;                
    Uint16 ASYNC_COND_EN:1;             
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:16;                    
};

union CLB_OUTPUT_COND_CTRL_2_REG {
    Uint32  all;
    struct  CLB_OUTPUT_COND_CTRL_2_BITS  bit;
};

struct CLB_OUTPUT_COND_CTRL_3_BITS {    
    Uint16 LEVEL_1_SEL:1;               
    Uint16 LEVEL_2_SEL:2;               
    Uint16 LEVEL_3_SEL:2;               
    Uint16 SEL_GATING_CTRL:3;           
    Uint16 SEL_RELEASE_CTRL:3;          
    Uint16 HW_GATING_CTRL_SEL:1;        
    Uint16 HW_RLS_CTRL_SEL:1;           
    Uint16 SEL_RAW_IN:1;                
    Uint16 ASYNC_COND_EN:1;             
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:16;                    
};

union CLB_OUTPUT_COND_CTRL_3_REG {
    Uint32  all;
    struct  CLB_OUTPUT_COND_CTRL_3_BITS  bit;
};

struct CLB_OUTPUT_COND_CTRL_4_BITS {    
    Uint16 LEVEL_1_SEL:1;               
    Uint16 LEVEL_2_SEL:2;               
    Uint16 LEVEL_3_SEL:2;               
    Uint16 SEL_GATING_CTRL:3;           
    Uint16 SEL_RELEASE_CTRL:3;          
    Uint16 HW_GATING_CTRL_SEL:1;        
    Uint16 HW_RLS_CTRL_SEL:1;           
    Uint16 SEL_RAW_IN:1;                
    Uint16 ASYNC_COND_EN:1;             
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:16;                    
};

union CLB_OUTPUT_COND_CTRL_4_REG {
    Uint32  all;
    struct  CLB_OUTPUT_COND_CTRL_4_BITS  bit;
};

struct CLB_OUTPUT_COND_CTRL_5_BITS {    
    Uint16 LEVEL_1_SEL:1;               
    Uint16 LEVEL_2_SEL:2;               
    Uint16 LEVEL_3_SEL:2;               
    Uint16 SEL_GATING_CTRL:3;           
    Uint16 SEL_RELEASE_CTRL:3;          
    Uint16 HW_GATING_CTRL_SEL:1;        
    Uint16 HW_RLS_CTRL_SEL:1;           
    Uint16 SEL_RAW_IN:1;                
    Uint16 ASYNC_COND_EN:1;             
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:16;                    
};

union CLB_OUTPUT_COND_CTRL_5_REG {
    Uint32  all;
    struct  CLB_OUTPUT_COND_CTRL_5_BITS  bit;
};

struct CLB_OUTPUT_COND_CTRL_6_BITS {    
    Uint16 LEVEL_1_SEL:1;               
    Uint16 LEVEL_2_SEL:2;               
    Uint16 LEVEL_3_SEL:2;               
    Uint16 SEL_GATING_CTRL:3;           
    Uint16 SEL_RELEASE_CTRL:3;          
    Uint16 HW_GATING_CTRL_SEL:1;        
    Uint16 HW_RLS_CTRL_SEL:1;           
    Uint16 SEL_RAW_IN:1;                
    Uint16 ASYNC_COND_EN:1;             
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:16;                    
};

union CLB_OUTPUT_COND_CTRL_6_REG {
    Uint32  all;
    struct  CLB_OUTPUT_COND_CTRL_6_BITS  bit;
};

struct CLB_OUTPUT_COND_CTRL_7_BITS {    
    Uint16 LEVEL_1_SEL:1;               
    Uint16 LEVEL_2_SEL:2;               
    Uint16 LEVEL_3_SEL:2;               
    Uint16 SEL_GATING_CTRL:3;           
    Uint16 SEL_RELEASE_CTRL:3;          
    Uint16 HW_GATING_CTRL_SEL:1;        
    Uint16 HW_RLS_CTRL_SEL:1;           
    Uint16 SEL_RAW_IN:1;                
    Uint16 ASYNC_COND_EN:1;             
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:16;                    
};

union CLB_OUTPUT_COND_CTRL_7_REG {
    Uint32  all;
    struct  CLB_OUTPUT_COND_CTRL_7_BITS  bit;
};

struct CLB_MISC_ACCESS_CTRL_BITS {      
    Uint16 SPIEN:1;                     
    Uint16 BLKEN:1;                     
    Uint16 rsvd1:14;                    
};

union CLB_MISC_ACCESS_CTRL_REG {
    Uint16  all;
    struct  CLB_MISC_ACCESS_CTRL_BITS  bit;
};

struct CLB_SPI_DATA_CTRL_HI_BITS {      
    Uint16 STRB:5;                      
    Uint16 rsvd1:3;                     
    Uint16 SHIFT:5;                     
    Uint16 rsvd2:3;                     
};

union CLB_SPI_DATA_CTRL_HI_REG {
    Uint16  all;
    struct  CLB_SPI_DATA_CTRL_HI_BITS  bit;
};

struct CLB_LOGIC_CONFIG_REGS {
    Uint16                                   rsvd1[2];                     
    union   CLB_COUNT_RESET_REG              CLB_COUNT_RESET;              
    union   CLB_COUNT_MODE_1_REG             CLB_COUNT_MODE_1;             
    union   CLB_COUNT_MODE_0_REG             CLB_COUNT_MODE_0;             
    union   CLB_COUNT_EVENT_REG              CLB_COUNT_EVENT;              
    union   CLB_FSM_EXTRA_IN0_REG            CLB_FSM_EXTRA_IN0;            
    union   CLB_FSM_EXTERNAL_IN0_REG         CLB_FSM_EXTERNAL_IN0;         
    union   CLB_FSM_EXTERNAL_IN1_REG         CLB_FSM_EXTERNAL_IN1;         
    union   CLB_FSM_EXTRA_IN1_REG            CLB_FSM_EXTRA_IN1;            
    union   CLB_LUT4_IN0_REG                 CLB_LUT4_IN0;                 
    union   CLB_LUT4_IN1_REG                 CLB_LUT4_IN1;                 
    union   CLB_LUT4_IN2_REG                 CLB_LUT4_IN2;                 
    union   CLB_LUT4_IN3_REG                 CLB_LUT4_IN3;                 
    Uint16                                   rsvd2[2];                     
    union   CLB_FSM_LUT_FN1_0_REG            CLB_FSM_LUT_FN1_0;            
    union   CLB_FSM_LUT_FN2_REG              CLB_FSM_LUT_FN2;              
    union   CLB_LUT4_FN1_0_REG               CLB_LUT4_FN1_0;               
    union   CLB_LUT4_FN2_REG                 CLB_LUT4_FN2;                 
    union   CLB_FSM_NEXT_STATE_0_REG         CLB_FSM_NEXT_STATE_0;         
    union   CLB_FSM_NEXT_STATE_1_REG         CLB_FSM_NEXT_STATE_1;         
    union   CLB_FSM_NEXT_STATE_2_REG         CLB_FSM_NEXT_STATE_2;         
    union   CLB_MISC_CONTROL_REG             CLB_MISC_CONTROL;             
    union   CLB_OUTPUT_LUT_0_REG             CLB_OUTPUT_LUT_0;             
    union   CLB_OUTPUT_LUT_1_REG             CLB_OUTPUT_LUT_1;             
    union   CLB_OUTPUT_LUT_2_REG             CLB_OUTPUT_LUT_2;             
    union   CLB_OUTPUT_LUT_3_REG             CLB_OUTPUT_LUT_3;             
    union   CLB_OUTPUT_LUT_4_REG             CLB_OUTPUT_LUT_4;             
    union   CLB_OUTPUT_LUT_5_REG             CLB_OUTPUT_LUT_5;             
    union   CLB_OUTPUT_LUT_6_REG             CLB_OUTPUT_LUT_6;             
    union   CLB_OUTPUT_LUT_7_REG             CLB_OUTPUT_LUT_7;             
    union   CLB_HLC_EVENT_SEL_REG            CLB_HLC_EVENT_SEL;            
    union   CLB_COUNT_MATCH_TAP_SEL_REG      CLB_COUNT_MATCH_TAP_SEL;      
    union   CLB_OUTPUT_COND_CTRL_0_REG       CLB_OUTPUT_COND_CTRL_0;       
    union   CLB_OUTPUT_COND_CTRL_1_REG       CLB_OUTPUT_COND_CTRL_1;       
    union   CLB_OUTPUT_COND_CTRL_2_REG       CLB_OUTPUT_COND_CTRL_2;       
    union   CLB_OUTPUT_COND_CTRL_3_REG       CLB_OUTPUT_COND_CTRL_3;       
    union   CLB_OUTPUT_COND_CTRL_4_REG       CLB_OUTPUT_COND_CTRL_4;       
    union   CLB_OUTPUT_COND_CTRL_5_REG       CLB_OUTPUT_COND_CTRL_5;       
    union   CLB_OUTPUT_COND_CTRL_6_REG       CLB_OUTPUT_COND_CTRL_6;       
    union   CLB_OUTPUT_COND_CTRL_7_REG       CLB_OUTPUT_COND_CTRL_7;       
    union   CLB_MISC_ACCESS_CTRL_REG         CLB_MISC_ACCESS_CTRL;         
    union   CLB_SPI_DATA_CTRL_HI_REG         CLB_SPI_DATA_CTRL_HI;         
};

struct CLB_LOAD_EN_BITS {               
    Uint16 LOAD_EN:1;                   
    Uint16 GLOBAL_EN:1;                 
    Uint16 STOP:1;                      
    Uint16 NMI_EN:1;                    
    Uint16 PIPELINE_EN:1;               
    Uint16 rsvd1:11;                    
};

union CLB_LOAD_EN_REG {
    Uint16  all;
    struct  CLB_LOAD_EN_BITS  bit;
};

struct CLB_LOAD_ADDR_BITS {             
    Uint16 ADDR:6;                      
    Uint16 rsvd1:10;                    
    Uint16 rsvd2:16;                    
};

union CLB_LOAD_ADDR_REG {
    Uint32  all;
    struct  CLB_LOAD_ADDR_BITS  bit;
};

struct CLB_INPUT_FILTER_BITS {          
    Uint16 FIN0:2;                      
    Uint16 FIN1:2;                      
    Uint16 FIN2:2;                      
    Uint16 FIN3:2;                      
    Uint16 FIN4:2;                      
    Uint16 FIN5:2;                      
    Uint16 FIN6:2;                      
    Uint16 FIN7:2;                      
    Uint16 SYNC0:1;                     
    Uint16 SYNC1:1;                     
    Uint16 SYNC2:1;                     
    Uint16 SYNC3:1;                     
    Uint16 SYNC4:1;                     
    Uint16 SYNC5:1;                     
    Uint16 SYNC6:1;                     
    Uint16 SYNC7:1;                     
    Uint16 PIPE0:1;                     
    Uint16 PIPE1:1;                     
    Uint16 PIPE2:1;                     
    Uint16 PIPE3:1;                     
    Uint16 PIPE4:1;                     
    Uint16 PIPE5:1;                     
    Uint16 PIPE6:1;                     
    Uint16 PIPE7:1;                     
};

union CLB_INPUT_FILTER_REG {
    Uint32  all;
    struct  CLB_INPUT_FILTER_BITS  bit;
};

struct CLB_IN_MUX_SEL_0_BITS {          
    Uint16 SEL_GP_IN_0:1;               
    Uint16 SEL_GP_IN_1:1;               
    Uint16 SEL_GP_IN_2:1;               
    Uint16 SEL_GP_IN_3:1;               
    Uint16 SEL_GP_IN_4:1;               
    Uint16 SEL_GP_IN_5:1;               
    Uint16 SEL_GP_IN_6:1;               
    Uint16 SEL_GP_IN_7:1;               
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union CLB_IN_MUX_SEL_0_REG {
    Uint32  all;
    struct  CLB_IN_MUX_SEL_0_BITS  bit;
};

struct CLB_LCL_MUX_SEL_1_BITS {         
    Uint16 LCL_MUX_SEL_IN_0:5;          
    Uint16 LCL_MUX_SEL_IN_1:5;          
    Uint16 LCL_MUX_SEL_IN_2:5;          
    Uint32 LCL_MUX_SEL_IN_3:5;          
    Uint16 rsvd1:8;                     
    Uint16 MISC_INPUT_SEL_0:1;          
    Uint16 MISC_INPUT_SEL_1:1;          
    Uint16 MISC_INPUT_SEL_2:1;          
    Uint16 MISC_INPUT_SEL_3:1;          
};

union CLB_LCL_MUX_SEL_1_REG {
    Uint32  all;
    struct  CLB_LCL_MUX_SEL_1_BITS  bit;
};

struct CLB_LCL_MUX_SEL_2_BITS {         
    Uint16 LCL_MUX_SEL_IN_4:5;          
    Uint16 LCL_MUX_SEL_IN_5:5;          
    Uint16 LCL_MUX_SEL_IN_6:5;          
    Uint32 LCL_MUX_SEL_IN_7:5;          
    Uint16 rsvd1:8;                     
    Uint16 MISC_INPUT_SEL_4:1;          
    Uint16 MISC_INPUT_SEL_5:1;          
    Uint16 MISC_INPUT_SEL_6:1;          
    Uint16 MISC_INPUT_SEL_7:1;          
};

union CLB_LCL_MUX_SEL_2_REG {
    Uint32  all;
    struct  CLB_LCL_MUX_SEL_2_BITS  bit;
};

struct CLB_BUF_PTR_BITS {               
    Uint16 PULL:8;                      
    Uint16 rsvd1:8;                     
    Uint16 PUSH:8;                      
    Uint16 rsvd2:8;                     
};

union CLB_BUF_PTR_REG {
    Uint32  all;
    struct  CLB_BUF_PTR_BITS  bit;
};

struct CLB_GP_REG_BITS {                
    Uint16 REG:8;                       
    Uint16 rsvd1:8;                     
    Uint16 SW_GATING_CTRL_0:1;          
    Uint16 SW_GATING_CTRL_1:1;          
    Uint16 SW_GATING_CTRL_2:1;          
    Uint16 SW_GATING_CTRL_3:1;          
    Uint16 SW_GATING_CTRL_4:1;          
    Uint16 SW_GATING_CTRL_5:1;          
    Uint16 SW_GATING_CTRL_6:1;          
    Uint16 SW_GATING_CTRL_7:1;          
    Uint16 SW_RLS_CTRL_0:1;             
    Uint16 SW_RLS_CTRL_1:1;             
    Uint16 SW_RLS_CTRL_2:1;             
    Uint16 SW_RLS_CTRL_3:1;             
    Uint16 SW_RLS_CTRL_4:1;             
    Uint16 SW_RLS_CTRL_5:1;             
    Uint16 SW_RLS_CTRL_6:1;             
    Uint16 SW_RLS_CTRL_7:1;             
};

union CLB_GP_REG_REG {
    Uint32  all;
    struct  CLB_GP_REG_BITS  bit;
};

struct CLB_GLBL_MUX_SEL_1_BITS {        
    Uint16 GLBL_MUX_SEL_IN_0:7;         
    Uint16 GLBL_MUX_SEL_IN_1:7;         
    Uint32 GLBL_MUX_SEL_IN_2:7;         
    Uint16 GLBL_MUX_SEL_IN_3:7;         
    Uint16 rsvd1:4;                     
};

union CLB_GLBL_MUX_SEL_1_REG {
    Uint32  all;
    struct  CLB_GLBL_MUX_SEL_1_BITS  bit;
};

struct CLB_GLBL_MUX_SEL_2_BITS {        
    Uint16 GLBL_MUX_SEL_IN_4:7;         
    Uint16 GLBL_MUX_SEL_IN_5:7;         
    Uint32 GLBL_MUX_SEL_IN_6:7;         
    Uint16 GLBL_MUX_SEL_IN_7:7;         
    Uint16 rsvd1:4;                     
};

union CLB_GLBL_MUX_SEL_2_REG {
    Uint32  all;
    struct  CLB_GLBL_MUX_SEL_2_BITS  bit;
};

struct CLB_PRESCALE_CTRL_BITS {         
    Uint16 CLKEN:1;                     
    Uint16 STRB:1;                      
    Uint16 TAP:4;                       
    Uint16 rsvd1:10;                    
    Uint16 PRESCALE:16;                 
};

union CLB_PRESCALE_CTRL_REG {
    Uint32  all;
    struct  CLB_PRESCALE_CTRL_BITS  bit;
};

struct CLB_INTR_TAG_REG_BITS {          
    Uint16 TAG:6;                       
    Uint16 rsvd1:10;                    
};

union CLB_INTR_TAG_REG_REG {
    Uint16  all;
    struct  CLB_INTR_TAG_REG_BITS  bit;
};

struct CLB_LOCK_BITS {                  
    Uint16 LOCK:1;                      
    Uint16 rsvd1:15;                    
    Uint16 KEY:16;                      
};

union CLB_LOCK_REG {
    Uint32  all;
    struct  CLB_LOCK_BITS  bit;
};

struct CLB_HLC_INSTR_READ_PTR_BITS {    
    Uint16 READ_PTR:5;                  
    Uint16 rsvd1:11;                    
};

union CLB_HLC_INSTR_READ_PTR_REG {
    Uint16  all;
    struct  CLB_HLC_INSTR_READ_PTR_BITS  bit;
};

struct CLB_HLC_INSTR_VALUE_BITS {       
    Uint16 INSTR:12;                    
    Uint16 rsvd1:4;                     
};

union CLB_HLC_INSTR_VALUE_REG {
    Uint16  all;
    struct  CLB_HLC_INSTR_VALUE_BITS  bit;
};

struct CLB_DBG_OUT_2_BITS {             
    Uint16 OUT:8;                       
    Uint16 IN:8;                        
    Uint16 rsvd1:16;                    
};

union CLB_DBG_OUT_2_REG {
    Uint32  all;
    struct  CLB_DBG_OUT_2_BITS  bit;
};

struct CLB_DBG_OUT_BITS {               
    Uint16 rsvd1:1;                     
    Uint16 COUNT0_MATCH2:1;             
    Uint16 COUNT0_ZERO:1;               
    Uint16 COUNT0_MATCH1:1;             
    Uint16 FSM0_S0:1;                   
    Uint16 FSM0_S1:1;                   
    Uint16 FSM0_LUTOUT:1;               
    Uint16 LUT40_OUT:1;                 
    Uint16 rsvd2:1;                     
    Uint16 COUNT1_MATCH2:1;             
    Uint16 COUNT1_ZERO:1;               
    Uint16 COUNT1_MATCH1:1;             
    Uint16 FSM1_S0:1;                   
    Uint16 FSM1_S1:1;                   
    Uint16 FSM1_LUTOUT:1;               
    Uint16 LUT41_OUT:1;                 
    Uint16 rsvd3:1;                     
    Uint16 COUNT2_MATCH2:1;             
    Uint16 COUNT2_ZERO:1;               
    Uint16 COUNT2_MATCH1:1;             
    Uint16 FSM2_S0:1;                   
    Uint16 FSM2_S1:1;                   
    Uint16 FSM2_LUTOUT:1;               
    Uint16 LUT42_OUT:1;                 
    Uint16 OUT0:1;                      
    Uint16 OUT1:1;                      
    Uint16 OUT2:1;                      
    Uint16 OUT3:1;                      
    Uint16 OUT4:1;                      
    Uint16 OUT5:1;                      
    Uint16 OUT6:1;                      
    Uint16 OUT7:1;                      
};

union CLB_DBG_OUT_REG {
    Uint32  all;
    struct  CLB_DBG_OUT_BITS  bit;
};

struct CLB_LOGIC_CONTROL_REGS {
    union   CLB_LOAD_EN_REG                  CLB_LOAD_EN;                  
    Uint16                                   rsvd1;                        
    union   CLB_LOAD_ADDR_REG                CLB_LOAD_ADDR;                
    Uint32                                   CLB_LOAD_DATA;                
    union   CLB_INPUT_FILTER_REG             CLB_INPUT_FILTER;             
    union   CLB_IN_MUX_SEL_0_REG             CLB_IN_MUX_SEL_0;             
    union   CLB_LCL_MUX_SEL_1_REG            CLB_LCL_MUX_SEL_1;            
    union   CLB_LCL_MUX_SEL_2_REG            CLB_LCL_MUX_SEL_2;            
    union   CLB_BUF_PTR_REG                  CLB_BUF_PTR;                  
    union   CLB_GP_REG_REG                   CLB_GP_REG;                   
    Uint32                                   CLB_OUT_EN;                   
    union   CLB_GLBL_MUX_SEL_1_REG           CLB_GLBL_MUX_SEL_1;           
    union   CLB_GLBL_MUX_SEL_2_REG           CLB_GLBL_MUX_SEL_2;           
    union   CLB_PRESCALE_CTRL_REG            CLB_PRESCALE_CTRL;            
    Uint16                                   rsvd2[6];                     
    union   CLB_INTR_TAG_REG_REG             CLB_INTR_TAG_REG;             
    Uint16                                   rsvd3;                        
    union   CLB_LOCK_REG                     CLB_LOCK;                     
    union   CLB_HLC_INSTR_READ_PTR_REG       CLB_HLC_INSTR_READ_PTR;       
    Uint16                                   rsvd4;                        
    union   CLB_HLC_INSTR_VALUE_REG          CLB_HLC_INSTR_VALUE;          
    Uint16                                   rsvd5[7];                     
    union   CLB_DBG_OUT_2_REG                CLB_DBG_OUT_2;                
    Uint32                                   CLB_DBG_R0;                   
    Uint32                                   CLB_DBG_R1;                   
    Uint32                                   CLB_DBG_R2;                   
    Uint32                                   CLB_DBG_R3;                   
    Uint32                                   CLB_DBG_C0;                   
    Uint32                                   CLB_DBG_C1;                   
    Uint32                                   CLB_DBG_C2;                   
    union   CLB_DBG_OUT_REG                  CLB_DBG_OUT;                  
};

struct CLB_DATA_EXCHANGE_REGS {
    Uint32                                   CLB_PUSH[4];                  
    Uint16                                   rsvd1[56];                    
    Uint32                                   CLB_PULL[4];                  
};




extern volatile struct CLB_LOGIC_CONFIG_REGS Clb1LogicCfgRegs;
extern volatile struct CLB_LOGIC_CONTROL_REGS Clb1LogicCtrlRegs;
extern volatile struct CLB_DATA_EXCHANGE_REGS Clb1DataExchRegs;
extern volatile struct CLB_LOGIC_CONFIG_REGS Clb2LogicCfgRegs;
extern volatile struct CLB_LOGIC_CONTROL_REGS Clb2LogicCtrlRegs;
extern volatile struct CLB_DATA_EXCHANGE_REGS Clb2DataExchRegs;
extern volatile struct CLB_LOGIC_CONFIG_REGS Clb3LogicCfgRegs;
extern volatile struct CLB_LOGIC_CONTROL_REGS Clb3LogicCtrlRegs;
extern volatile struct CLB_DATA_EXCHANGE_REGS Clb3DataExchRegs;
extern volatile struct CLB_LOGIC_CONFIG_REGS Clb4LogicCfgRegs;
extern volatile struct CLB_LOGIC_CONTROL_REGS Clb4LogicCtrlRegs;
extern volatile struct CLB_DATA_EXCHANGE_REGS Clb4DataExchRegs;



















































struct AUXSIG0MUX0TO15CFG_BITS {        
    Uint16 MUX0:2;                      
    Uint16 MUX1:2;                      
    Uint16 MUX2:2;                      
    Uint16 MUX3:2;                      
    Uint16 MUX4:2;                      
    Uint16 MUX5:2;                      
    Uint16 MUX6:2;                      
    Uint16 MUX7:2;                      
    Uint16 MUX8:2;                      
    Uint16 MUX9:2;                      
    Uint16 MUX10:2;                     
    Uint16 MUX11:2;                     
    Uint16 MUX12:2;                     
    Uint16 MUX13:2;                     
    Uint16 MUX14:2;                     
    Uint16 MUX15:2;                     
};

union AUXSIG0MUX0TO15CFG_REG {
    Uint32  all;
    struct  AUXSIG0MUX0TO15CFG_BITS  bit;
};

struct AUXSIG0MUX16TO31CFG_BITS {       
    Uint16 MUX16:2;                     
    Uint16 MUX17:2;                     
    Uint16 MUX18:2;                     
    Uint16 MUX19:2;                     
    Uint16 MUX20:2;                     
    Uint16 MUX21:2;                     
    Uint16 MUX22:2;                     
    Uint16 MUX23:2;                     
    Uint16 MUX24:2;                     
    Uint16 MUX25:2;                     
    Uint16 MUX26:2;                     
    Uint16 MUX27:2;                     
    Uint16 MUX28:2;                     
    Uint16 MUX29:2;                     
    Uint16 MUX30:2;                     
    Uint16 MUX31:2;                     
};

union AUXSIG0MUX16TO31CFG_REG {
    Uint32  all;
    struct  AUXSIG0MUX16TO31CFG_BITS  bit;
};

struct AUXSIG1MUX0TO15CFG_BITS {        
    Uint16 MUX0:2;                      
    Uint16 MUX1:2;                      
    Uint16 MUX2:2;                      
    Uint16 MUX3:2;                      
    Uint16 MUX4:2;                      
    Uint16 MUX5:2;                      
    Uint16 MUX6:2;                      
    Uint16 MUX7:2;                      
    Uint16 MUX8:2;                      
    Uint16 MUX9:2;                      
    Uint16 MUX10:2;                     
    Uint16 MUX11:2;                     
    Uint16 MUX12:2;                     
    Uint16 MUX13:2;                     
    Uint16 MUX14:2;                     
    Uint16 MUX15:2;                     
};

union AUXSIG1MUX0TO15CFG_REG {
    Uint32  all;
    struct  AUXSIG1MUX0TO15CFG_BITS  bit;
};

struct AUXSIG1MUX16TO31CFG_BITS {       
    Uint16 MUX16:2;                     
    Uint16 MUX17:2;                     
    Uint16 MUX18:2;                     
    Uint16 MUX19:2;                     
    Uint16 MUX20:2;                     
    Uint16 MUX21:2;                     
    Uint16 MUX22:2;                     
    Uint16 MUX23:2;                     
    Uint16 MUX24:2;                     
    Uint16 MUX25:2;                     
    Uint16 MUX26:2;                     
    Uint16 MUX27:2;                     
    Uint16 MUX28:2;                     
    Uint16 MUX29:2;                     
    Uint16 MUX30:2;                     
    Uint16 MUX31:2;                     
};

union AUXSIG1MUX16TO31CFG_REG {
    Uint32  all;
    struct  AUXSIG1MUX16TO31CFG_BITS  bit;
};

struct AUXSIG2MUX0TO15CFG_BITS {        
    Uint16 MUX0:2;                      
    Uint16 MUX1:2;                      
    Uint16 MUX2:2;                      
    Uint16 MUX3:2;                      
    Uint16 MUX4:2;                      
    Uint16 MUX5:2;                      
    Uint16 MUX6:2;                      
    Uint16 MUX7:2;                      
    Uint16 MUX8:2;                      
    Uint16 MUX9:2;                      
    Uint16 MUX10:2;                     
    Uint16 MUX11:2;                     
    Uint16 MUX12:2;                     
    Uint16 MUX13:2;                     
    Uint16 MUX14:2;                     
    Uint16 MUX15:2;                     
};

union AUXSIG2MUX0TO15CFG_REG {
    Uint32  all;
    struct  AUXSIG2MUX0TO15CFG_BITS  bit;
};

struct AUXSIG2MUX16TO31CFG_BITS {       
    Uint16 MUX16:2;                     
    Uint16 MUX17:2;                     
    Uint16 MUX18:2;                     
    Uint16 MUX19:2;                     
    Uint16 MUX20:2;                     
    Uint16 MUX21:2;                     
    Uint16 MUX22:2;                     
    Uint16 MUX23:2;                     
    Uint16 MUX24:2;                     
    Uint16 MUX25:2;                     
    Uint16 MUX26:2;                     
    Uint16 MUX27:2;                     
    Uint16 MUX28:2;                     
    Uint16 MUX29:2;                     
    Uint16 MUX30:2;                     
    Uint16 MUX31:2;                     
};

union AUXSIG2MUX16TO31CFG_REG {
    Uint32  all;
    struct  AUXSIG2MUX16TO31CFG_BITS  bit;
};

struct AUXSIG3MUX0TO15CFG_BITS {        
    Uint16 MUX0:2;                      
    Uint16 MUX1:2;                      
    Uint16 MUX2:2;                      
    Uint16 MUX3:2;                      
    Uint16 MUX4:2;                      
    Uint16 MUX5:2;                      
    Uint16 MUX6:2;                      
    Uint16 MUX7:2;                      
    Uint16 MUX8:2;                      
    Uint16 MUX9:2;                      
    Uint16 MUX10:2;                     
    Uint16 MUX11:2;                     
    Uint16 MUX12:2;                     
    Uint16 MUX13:2;                     
    Uint16 MUX14:2;                     
    Uint16 MUX15:2;                     
};

union AUXSIG3MUX0TO15CFG_REG {
    Uint32  all;
    struct  AUXSIG3MUX0TO15CFG_BITS  bit;
};

struct AUXSIG3MUX16TO31CFG_BITS {       
    Uint16 MUX16:2;                     
    Uint16 MUX17:2;                     
    Uint16 MUX18:2;                     
    Uint16 MUX19:2;                     
    Uint16 MUX20:2;                     
    Uint16 MUX21:2;                     
    Uint16 MUX22:2;                     
    Uint16 MUX23:2;                     
    Uint16 MUX24:2;                     
    Uint16 MUX25:2;                     
    Uint16 MUX26:2;                     
    Uint16 MUX27:2;                     
    Uint16 MUX28:2;                     
    Uint16 MUX29:2;                     
    Uint16 MUX30:2;                     
    Uint16 MUX31:2;                     
};

union AUXSIG3MUX16TO31CFG_REG {
    Uint32  all;
    struct  AUXSIG3MUX16TO31CFG_BITS  bit;
};

struct AUXSIG4MUX0TO15CFG_BITS {        
    Uint16 MUX0:2;                      
    Uint16 MUX1:2;                      
    Uint16 MUX2:2;                      
    Uint16 MUX3:2;                      
    Uint16 MUX4:2;                      
    Uint16 MUX5:2;                      
    Uint16 MUX6:2;                      
    Uint16 MUX7:2;                      
    Uint16 MUX8:2;                      
    Uint16 MUX9:2;                      
    Uint16 MUX10:2;                     
    Uint16 MUX11:2;                     
    Uint16 MUX12:2;                     
    Uint16 MUX13:2;                     
    Uint16 MUX14:2;                     
    Uint16 MUX15:2;                     
};

union AUXSIG4MUX0TO15CFG_REG {
    Uint32  all;
    struct  AUXSIG4MUX0TO15CFG_BITS  bit;
};

struct AUXSIG4MUX16TO31CFG_BITS {       
    Uint16 MUX16:2;                     
    Uint16 MUX17:2;                     
    Uint16 MUX18:2;                     
    Uint16 MUX19:2;                     
    Uint16 MUX20:2;                     
    Uint16 MUX21:2;                     
    Uint16 MUX22:2;                     
    Uint16 MUX23:2;                     
    Uint16 MUX24:2;                     
    Uint16 MUX25:2;                     
    Uint16 MUX26:2;                     
    Uint16 MUX27:2;                     
    Uint16 MUX28:2;                     
    Uint16 MUX29:2;                     
    Uint16 MUX30:2;                     
    Uint16 MUX31:2;                     
};

union AUXSIG4MUX16TO31CFG_REG {
    Uint32  all;
    struct  AUXSIG4MUX16TO31CFG_BITS  bit;
};

struct AUXSIG5MUX0TO15CFG_BITS {        
    Uint16 MUX0:2;                      
    Uint16 MUX1:2;                      
    Uint16 MUX2:2;                      
    Uint16 MUX3:2;                      
    Uint16 MUX4:2;                      
    Uint16 MUX5:2;                      
    Uint16 MUX6:2;                      
    Uint16 MUX7:2;                      
    Uint16 MUX8:2;                      
    Uint16 MUX9:2;                      
    Uint16 MUX10:2;                     
    Uint16 MUX11:2;                     
    Uint16 MUX12:2;                     
    Uint16 MUX13:2;                     
    Uint16 MUX14:2;                     
    Uint16 MUX15:2;                     
};

union AUXSIG5MUX0TO15CFG_REG {
    Uint32  all;
    struct  AUXSIG5MUX0TO15CFG_BITS  bit;
};

struct AUXSIG5MUX16TO31CFG_BITS {       
    Uint16 MUX16:2;                     
    Uint16 MUX17:2;                     
    Uint16 MUX18:2;                     
    Uint16 MUX19:2;                     
    Uint16 MUX20:2;                     
    Uint16 MUX21:2;                     
    Uint16 MUX22:2;                     
    Uint16 MUX23:2;                     
    Uint16 MUX24:2;                     
    Uint16 MUX25:2;                     
    Uint16 MUX26:2;                     
    Uint16 MUX27:2;                     
    Uint16 MUX28:2;                     
    Uint16 MUX29:2;                     
    Uint16 MUX30:2;                     
    Uint16 MUX31:2;                     
};

union AUXSIG5MUX16TO31CFG_REG {
    Uint32  all;
    struct  AUXSIG5MUX16TO31CFG_BITS  bit;
};

struct AUXSIG6MUX0TO15CFG_BITS {        
    Uint16 MUX0:2;                      
    Uint16 MUX1:2;                      
    Uint16 MUX2:2;                      
    Uint16 MUX3:2;                      
    Uint16 MUX4:2;                      
    Uint16 MUX5:2;                      
    Uint16 MUX6:2;                      
    Uint16 MUX7:2;                      
    Uint16 MUX8:2;                      
    Uint16 MUX9:2;                      
    Uint16 MUX10:2;                     
    Uint16 MUX11:2;                     
    Uint16 MUX12:2;                     
    Uint16 MUX13:2;                     
    Uint16 MUX14:2;                     
    Uint16 MUX15:2;                     
};

union AUXSIG6MUX0TO15CFG_REG {
    Uint32  all;
    struct  AUXSIG6MUX0TO15CFG_BITS  bit;
};

struct AUXSIG6MUX16TO31CFG_BITS {       
    Uint16 MUX16:2;                     
    Uint16 MUX17:2;                     
    Uint16 MUX18:2;                     
    Uint16 MUX19:2;                     
    Uint16 MUX20:2;                     
    Uint16 MUX21:2;                     
    Uint16 MUX22:2;                     
    Uint16 MUX23:2;                     
    Uint16 MUX24:2;                     
    Uint16 MUX25:2;                     
    Uint16 MUX26:2;                     
    Uint16 MUX27:2;                     
    Uint16 MUX28:2;                     
    Uint16 MUX29:2;                     
    Uint16 MUX30:2;                     
    Uint16 MUX31:2;                     
};

union AUXSIG6MUX16TO31CFG_REG {
    Uint32  all;
    struct  AUXSIG6MUX16TO31CFG_BITS  bit;
};

struct AUXSIG7MUX0TO15CFG_BITS {        
    Uint16 MUX0:2;                      
    Uint16 MUX1:2;                      
    Uint16 MUX2:2;                      
    Uint16 MUX3:2;                      
    Uint16 MUX4:2;                      
    Uint16 MUX5:2;                      
    Uint16 MUX6:2;                      
    Uint16 MUX7:2;                      
    Uint16 MUX8:2;                      
    Uint16 MUX9:2;                      
    Uint16 MUX10:2;                     
    Uint16 MUX11:2;                     
    Uint16 MUX12:2;                     
    Uint16 MUX13:2;                     
    Uint16 MUX14:2;                     
    Uint16 MUX15:2;                     
};

union AUXSIG7MUX0TO15CFG_REG {
    Uint32  all;
    struct  AUXSIG7MUX0TO15CFG_BITS  bit;
};

struct AUXSIG7MUX16TO31CFG_BITS {       
    Uint16 MUX16:2;                     
    Uint16 MUX17:2;                     
    Uint16 MUX18:2;                     
    Uint16 MUX19:2;                     
    Uint16 MUX20:2;                     
    Uint16 MUX21:2;                     
    Uint16 MUX22:2;                     
    Uint16 MUX23:2;                     
    Uint16 MUX24:2;                     
    Uint16 MUX25:2;                     
    Uint16 MUX26:2;                     
    Uint16 MUX27:2;                     
    Uint16 MUX28:2;                     
    Uint16 MUX29:2;                     
    Uint16 MUX30:2;                     
    Uint16 MUX31:2;                     
};

union AUXSIG7MUX16TO31CFG_REG {
    Uint32  all;
    struct  AUXSIG7MUX16TO31CFG_BITS  bit;
};

struct AUXSIG0MUXENABLE_BITS {          
    Uint16 MUX0:1;                      
    Uint16 MUX1:1;                      
    Uint16 MUX2:1;                      
    Uint16 MUX3:1;                      
    Uint16 MUX4:1;                      
    Uint16 MUX5:1;                      
    Uint16 MUX6:1;                      
    Uint16 MUX7:1;                      
    Uint16 MUX8:1;                      
    Uint16 MUX9:1;                      
    Uint16 MUX10:1;                     
    Uint16 MUX11:1;                     
    Uint16 MUX12:1;                     
    Uint16 MUX13:1;                     
    Uint16 MUX14:1;                     
    Uint16 MUX15:1;                     
    Uint16 MUX16:1;                     
    Uint16 MUX17:1;                     
    Uint16 MUX18:1;                     
    Uint16 MUX19:1;                     
    Uint16 MUX20:1;                     
    Uint16 MUX21:1;                     
    Uint16 MUX22:1;                     
    Uint16 MUX23:1;                     
    Uint16 MUX24:1;                     
    Uint16 MUX25:1;                     
    Uint16 MUX26:1;                     
    Uint16 MUX27:1;                     
    Uint16 MUX28:1;                     
    Uint16 MUX29:1;                     
    Uint16 MUX30:1;                     
    Uint16 MUX31:1;                     
};

union AUXSIG0MUXENABLE_REG {
    Uint32  all;
    struct  AUXSIG0MUXENABLE_BITS  bit;
};

struct AUXSIG1MUXENABLE_BITS {          
    Uint16 MUX0:1;                      
    Uint16 MUX1:1;                      
    Uint16 MUX2:1;                      
    Uint16 MUX3:1;                      
    Uint16 MUX4:1;                      
    Uint16 MUX5:1;                      
    Uint16 MUX6:1;                      
    Uint16 MUX7:1;                      
    Uint16 MUX8:1;                      
    Uint16 MUX9:1;                      
    Uint16 MUX10:1;                     
    Uint16 MUX11:1;                     
    Uint16 MUX12:1;                     
    Uint16 MUX13:1;                     
    Uint16 MUX14:1;                     
    Uint16 MUX15:1;                     
    Uint16 MUX16:1;                     
    Uint16 MUX17:1;                     
    Uint16 MUX18:1;                     
    Uint16 MUX19:1;                     
    Uint16 MUX20:1;                     
    Uint16 MUX21:1;                     
    Uint16 MUX22:1;                     
    Uint16 MUX23:1;                     
    Uint16 MUX24:1;                     
    Uint16 MUX25:1;                     
    Uint16 MUX26:1;                     
    Uint16 MUX27:1;                     
    Uint16 MUX28:1;                     
    Uint16 MUX29:1;                     
    Uint16 MUX30:1;                     
    Uint16 MUX31:1;                     
};

union AUXSIG1MUXENABLE_REG {
    Uint32  all;
    struct  AUXSIG1MUXENABLE_BITS  bit;
};

struct AUXSIG2MUXENABLE_BITS {          
    Uint16 MUX0:1;                      
    Uint16 MUX1:1;                      
    Uint16 MUX2:1;                      
    Uint16 MUX3:1;                      
    Uint16 MUX4:1;                      
    Uint16 MUX5:1;                      
    Uint16 MUX6:1;                      
    Uint16 MUX7:1;                      
    Uint16 MUX8:1;                      
    Uint16 MUX9:1;                      
    Uint16 MUX10:1;                     
    Uint16 MUX11:1;                     
    Uint16 MUX12:1;                     
    Uint16 MUX13:1;                     
    Uint16 MUX14:1;                     
    Uint16 MUX15:1;                     
    Uint16 MUX16:1;                     
    Uint16 MUX17:1;                     
    Uint16 MUX18:1;                     
    Uint16 MUX19:1;                     
    Uint16 MUX20:1;                     
    Uint16 MUX21:1;                     
    Uint16 MUX22:1;                     
    Uint16 MUX23:1;                     
    Uint16 MUX24:1;                     
    Uint16 MUX25:1;                     
    Uint16 MUX26:1;                     
    Uint16 MUX27:1;                     
    Uint16 MUX28:1;                     
    Uint16 MUX29:1;                     
    Uint16 MUX30:1;                     
    Uint16 MUX31:1;                     
};

union AUXSIG2MUXENABLE_REG {
    Uint32  all;
    struct  AUXSIG2MUXENABLE_BITS  bit;
};

struct AUXSIG3MUXENABLE_BITS {          
    Uint16 MUX0:1;                      
    Uint16 MUX1:1;                      
    Uint16 MUX2:1;                      
    Uint16 MUX3:1;                      
    Uint16 MUX4:1;                      
    Uint16 MUX5:1;                      
    Uint16 MUX6:1;                      
    Uint16 MUX7:1;                      
    Uint16 MUX8:1;                      
    Uint16 MUX9:1;                      
    Uint16 MUX10:1;                     
    Uint16 MUX11:1;                     
    Uint16 MUX12:1;                     
    Uint16 MUX13:1;                     
    Uint16 MUX14:1;                     
    Uint16 MUX15:1;                     
    Uint16 MUX16:1;                     
    Uint16 MUX17:1;                     
    Uint16 MUX18:1;                     
    Uint16 MUX19:1;                     
    Uint16 MUX20:1;                     
    Uint16 MUX21:1;                     
    Uint16 MUX22:1;                     
    Uint16 MUX23:1;                     
    Uint16 MUX24:1;                     
    Uint16 MUX25:1;                     
    Uint16 MUX26:1;                     
    Uint16 MUX27:1;                     
    Uint16 MUX28:1;                     
    Uint16 MUX29:1;                     
    Uint16 MUX30:1;                     
    Uint16 MUX31:1;                     
};

union AUXSIG3MUXENABLE_REG {
    Uint32  all;
    struct  AUXSIG3MUXENABLE_BITS  bit;
};

struct AUXSIG4MUXENABLE_BITS {          
    Uint16 MUX0:1;                      
    Uint16 MUX1:1;                      
    Uint16 MUX2:1;                      
    Uint16 MUX3:1;                      
    Uint16 MUX4:1;                      
    Uint16 MUX5:1;                      
    Uint16 MUX6:1;                      
    Uint16 MUX7:1;                      
    Uint16 MUX8:1;                      
    Uint16 MUX9:1;                      
    Uint16 MUX10:1;                     
    Uint16 MUX11:1;                     
    Uint16 MUX12:1;                     
    Uint16 MUX13:1;                     
    Uint16 MUX14:1;                     
    Uint16 MUX15:1;                     
    Uint16 MUX16:1;                     
    Uint16 MUX17:1;                     
    Uint16 MUX18:1;                     
    Uint16 MUX19:1;                     
    Uint16 MUX20:1;                     
    Uint16 MUX21:1;                     
    Uint16 MUX22:1;                     
    Uint16 MUX23:1;                     
    Uint16 MUX24:1;                     
    Uint16 MUX25:1;                     
    Uint16 MUX26:1;                     
    Uint16 MUX27:1;                     
    Uint16 MUX28:1;                     
    Uint16 MUX29:1;                     
    Uint16 MUX30:1;                     
    Uint16 MUX31:1;                     
};

union AUXSIG4MUXENABLE_REG {
    Uint32  all;
    struct  AUXSIG4MUXENABLE_BITS  bit;
};

struct AUXSIG5MUXENABLE_BITS {          
    Uint16 MUX0:1;                      
    Uint16 MUX1:1;                      
    Uint16 MUX2:1;                      
    Uint16 MUX3:1;                      
    Uint16 MUX4:1;                      
    Uint16 MUX5:1;                      
    Uint16 MUX6:1;                      
    Uint16 MUX7:1;                      
    Uint16 MUX8:1;                      
    Uint16 MUX9:1;                      
    Uint16 MUX10:1;                     
    Uint16 MUX11:1;                     
    Uint16 MUX12:1;                     
    Uint16 MUX13:1;                     
    Uint16 MUX14:1;                     
    Uint16 MUX15:1;                     
    Uint16 MUX16:1;                     
    Uint16 MUX17:1;                     
    Uint16 MUX18:1;                     
    Uint16 MUX19:1;                     
    Uint16 MUX20:1;                     
    Uint16 MUX21:1;                     
    Uint16 MUX22:1;                     
    Uint16 MUX23:1;                     
    Uint16 MUX24:1;                     
    Uint16 MUX25:1;                     
    Uint16 MUX26:1;                     
    Uint16 MUX27:1;                     
    Uint16 MUX28:1;                     
    Uint16 MUX29:1;                     
    Uint16 MUX30:1;                     
    Uint16 MUX31:1;                     
};

union AUXSIG5MUXENABLE_REG {
    Uint32  all;
    struct  AUXSIG5MUXENABLE_BITS  bit;
};

struct AUXSIG6MUXENABLE_BITS {          
    Uint16 MUX0:1;                      
    Uint16 MUX1:1;                      
    Uint16 MUX2:1;                      
    Uint16 MUX3:1;                      
    Uint16 MUX4:1;                      
    Uint16 MUX5:1;                      
    Uint16 MUX6:1;                      
    Uint16 MUX7:1;                      
    Uint16 MUX8:1;                      
    Uint16 MUX9:1;                      
    Uint16 MUX10:1;                     
    Uint16 MUX11:1;                     
    Uint16 MUX12:1;                     
    Uint16 MUX13:1;                     
    Uint16 MUX14:1;                     
    Uint16 MUX15:1;                     
    Uint16 MUX16:1;                     
    Uint16 MUX17:1;                     
    Uint16 MUX18:1;                     
    Uint16 MUX19:1;                     
    Uint16 MUX20:1;                     
    Uint16 MUX21:1;                     
    Uint16 MUX22:1;                     
    Uint16 MUX23:1;                     
    Uint16 MUX24:1;                     
    Uint16 MUX25:1;                     
    Uint16 MUX26:1;                     
    Uint16 MUX27:1;                     
    Uint16 MUX28:1;                     
    Uint16 MUX29:1;                     
    Uint16 MUX30:1;                     
    Uint16 MUX31:1;                     
};

union AUXSIG6MUXENABLE_REG {
    Uint32  all;
    struct  AUXSIG6MUXENABLE_BITS  bit;
};

struct AUXSIG7MUXENABLE_BITS {          
    Uint16 MUX0:1;                      
    Uint16 MUX1:1;                      
    Uint16 MUX2:1;                      
    Uint16 MUX3:1;                      
    Uint16 MUX4:1;                      
    Uint16 MUX5:1;                      
    Uint16 MUX6:1;                      
    Uint16 MUX7:1;                      
    Uint16 MUX8:1;                      
    Uint16 MUX9:1;                      
    Uint16 MUX10:1;                     
    Uint16 MUX11:1;                     
    Uint16 MUX12:1;                     
    Uint16 MUX13:1;                     
    Uint16 MUX14:1;                     
    Uint16 MUX15:1;                     
    Uint16 MUX16:1;                     
    Uint16 MUX17:1;                     
    Uint16 MUX18:1;                     
    Uint16 MUX19:1;                     
    Uint16 MUX20:1;                     
    Uint16 MUX21:1;                     
    Uint16 MUX22:1;                     
    Uint16 MUX23:1;                     
    Uint16 MUX24:1;                     
    Uint16 MUX25:1;                     
    Uint16 MUX26:1;                     
    Uint16 MUX27:1;                     
    Uint16 MUX28:1;                     
    Uint16 MUX29:1;                     
    Uint16 MUX30:1;                     
    Uint16 MUX31:1;                     
};

union AUXSIG7MUXENABLE_REG {
    Uint32  all;
    struct  AUXSIG7MUXENABLE_BITS  bit;
};

struct AUXSIGOUTINV_BITS {              
    Uint16 OUT0:1;                      
    Uint16 OUT1:1;                      
    Uint16 OUT2:1;                      
    Uint16 OUT3:1;                      
    Uint16 OUT4:1;                      
    Uint16 OUT5:1;                      
    Uint16 OUT6:1;                      
    Uint16 OUT7:1;                      
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union AUXSIGOUTINV_REG {
    Uint32  all;
    struct  AUXSIGOUTINV_BITS  bit;
};

struct AUXSIGLOCK_BITS {                
    Uint16 LOCK:1;                      
    Uint16 rsvd1:15;                    
    Uint16 KEY:16;                      
};

union AUXSIGLOCK_REG {
    Uint32  all;
    struct  AUXSIGLOCK_BITS  bit;
};

struct CLB_XBAR_REGS {
    union   AUXSIG0MUX0TO15CFG_REG           AUXSIG0MUX0TO15CFG;           
    union   AUXSIG0MUX16TO31CFG_REG          AUXSIG0MUX16TO31CFG;          
    union   AUXSIG1MUX0TO15CFG_REG           AUXSIG1MUX0TO15CFG;           
    union   AUXSIG1MUX16TO31CFG_REG          AUXSIG1MUX16TO31CFG;          
    union   AUXSIG2MUX0TO15CFG_REG           AUXSIG2MUX0TO15CFG;           
    union   AUXSIG2MUX16TO31CFG_REG          AUXSIG2MUX16TO31CFG;          
    union   AUXSIG3MUX0TO15CFG_REG           AUXSIG3MUX0TO15CFG;           
    union   AUXSIG3MUX16TO31CFG_REG          AUXSIG3MUX16TO31CFG;          
    union   AUXSIG4MUX0TO15CFG_REG           AUXSIG4MUX0TO15CFG;           
    union   AUXSIG4MUX16TO31CFG_REG          AUXSIG4MUX16TO31CFG;          
    union   AUXSIG5MUX0TO15CFG_REG           AUXSIG5MUX0TO15CFG;           
    union   AUXSIG5MUX16TO31CFG_REG          AUXSIG5MUX16TO31CFG;          
    union   AUXSIG6MUX0TO15CFG_REG           AUXSIG6MUX0TO15CFG;           
    union   AUXSIG6MUX16TO31CFG_REG          AUXSIG6MUX16TO31CFG;          
    union   AUXSIG7MUX0TO15CFG_REG           AUXSIG7MUX0TO15CFG;           
    union   AUXSIG7MUX16TO31CFG_REG          AUXSIG7MUX16TO31CFG;          
    union   AUXSIG0MUXENABLE_REG             AUXSIG0MUXENABLE;             
    union   AUXSIG1MUXENABLE_REG             AUXSIG1MUXENABLE;             
    union   AUXSIG2MUXENABLE_REG             AUXSIG2MUXENABLE;             
    union   AUXSIG3MUXENABLE_REG             AUXSIG3MUXENABLE;             
    union   AUXSIG4MUXENABLE_REG             AUXSIG4MUXENABLE;             
    union   AUXSIG5MUXENABLE_REG             AUXSIG5MUXENABLE;             
    union   AUXSIG6MUXENABLE_REG             AUXSIG6MUXENABLE;             
    union   AUXSIG7MUXENABLE_REG             AUXSIG7MUXENABLE;             
    Uint16                                   rsvd1[8];                     
    union   AUXSIGOUTINV_REG                 AUXSIGOUTINV;                 
    Uint16                                   rsvd2[4];                     
    union   AUXSIGLOCK_REG                   AUXSIGLOCK;                   
};




extern volatile struct CLB_XBAR_REGS ClbXbarRegs;



















































struct COMPCTL_BITS {                   
    Uint16 COMPHSOURCE:1;               
    Uint16 COMPHINV:1;                  
    Uint16 CTRIPHSEL:2;                 
    Uint16 CTRIPOUTHSEL:2;              
    Uint16 ASYNCHEN:1;                  
    Uint16 rsvd1:1;                     
    Uint16 COMPLSOURCE:1;               
    Uint16 COMPLINV:1;                  
    Uint16 CTRIPLSEL:2;                 
    Uint16 CTRIPOUTLSEL:2;              
    Uint16 ASYNCLEN:1;                  
    Uint16 COMPDACE:1;                  
};

union COMPCTL_REG {
    Uint16  all;
    struct  COMPCTL_BITS  bit;
};

struct COMPHYSCTL_BITS {                
    Uint16 COMPHYS:4;                   
    Uint16 rsvd1:12;                    
};

union COMPHYSCTL_REG {
    Uint16  all;
    struct  COMPHYSCTL_BITS  bit;
};

struct COMPSTS_BITS {                   
    Uint16 COMPHSTS:1;                  
    Uint16 COMPHLATCH:1;                
    Uint16 rsvd1:6;                     
    Uint16 COMPLSTS:1;                  
    Uint16 COMPLLATCH:1;                
    Uint16 rsvd2:6;                     
};

union COMPSTS_REG {
    Uint16  all;
    struct  COMPSTS_BITS  bit;
};

struct COMPSTSCLR_BITS {                
    Uint16 rsvd1:1;                     
    Uint16 HLATCHCLR:1;                 
    Uint16 HSYNCCLREN:1;                
    Uint16 rsvd2:6;                     
    Uint16 LLATCHCLR:1;                 
    Uint16 LSYNCCLREN:1;                
    Uint16 rsvd3:5;                     
};

union COMPSTSCLR_REG {
    Uint16  all;
    struct  COMPSTSCLR_BITS  bit;
};

struct COMPDACCTL_BITS {                
    Uint16 DACSOURCE:1;                 
    Uint16 RAMPSOURCE:4;                
    Uint16 SELREF:1;                    
    Uint16 RAMPLOADSEL:1;               
    Uint16 SWLOADSEL:1;                 
    Uint16 BLANKSOURCE:4;               
    Uint16 BLANKEN:1;                   
    Uint16 rsvd1:1;                     
    Uint16 FREESOFT:2;                  
};

union COMPDACCTL_REG {
    Uint16  all;
    struct  COMPDACCTL_BITS  bit;
};

struct DACHVALS_BITS {                  
    Uint16 DACVAL:12;                   
    Uint16 rsvd1:4;                     
};

union DACHVALS_REG {
    Uint16  all;
    struct  DACHVALS_BITS  bit;
};

struct DACHVALA_BITS {                  
    Uint16 DACVAL:12;                   
    Uint16 rsvd1:4;                     
};

union DACHVALA_REG {
    Uint16  all;
    struct  DACHVALA_BITS  bit;
};

struct DACLVALS_BITS {                  
    Uint16 DACVAL:12;                   
    Uint16 rsvd1:4;                     
};

union DACLVALS_REG {
    Uint16  all;
    struct  DACLVALS_BITS  bit;
};

struct DACLVALA_BITS {                  
    Uint16 DACVAL:12;                   
    Uint16 rsvd1:4;                     
};

union DACLVALA_REG {
    Uint16  all;
    struct  DACLVALA_BITS  bit;
};

struct RAMPDLYA_BITS {                  
    Uint16 DELAY:13;                    
    Uint16 rsvd1:3;                     
};

union RAMPDLYA_REG {
    Uint16  all;
    struct  RAMPDLYA_BITS  bit;
};

struct RAMPDLYS_BITS {                  
    Uint16 DELAY:13;                    
    Uint16 rsvd1:3;                     
};

union RAMPDLYS_REG {
    Uint16  all;
    struct  RAMPDLYS_BITS  bit;
};

struct CTRIPLFILCTL_BITS {              
    Uint16 rsvd1:4;                     
    Uint16 SAMPWIN:5;                   
    Uint16 THRESH:5;                    
    Uint16 rsvd2:1;                     
    Uint16 FILINIT:1;                   
};

union CTRIPLFILCTL_REG {
    Uint16  all;
    struct  CTRIPLFILCTL_BITS  bit;
};

struct CTRIPHFILCTL_BITS {              
    Uint16 rsvd1:4;                     
    Uint16 SAMPWIN:5;                   
    Uint16 THRESH:5;                    
    Uint16 rsvd2:1;                     
    Uint16 FILINIT:1;                   
};

union CTRIPHFILCTL_REG {
    Uint16  all;
    struct  CTRIPHFILCTL_BITS  bit;
};

struct COMPLOCK_BITS {                  
    Uint16 COMPCTL:1;                   
    Uint16 COMPHYSCTL:1;                
    Uint16 DACCTL:1;                    
    Uint16 CTRIP:1;                     
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:11;                    
};

union COMPLOCK_REG {
    Uint16  all;
    struct  COMPLOCK_BITS  bit;
};

struct CMPSS_REGS {
    union   COMPCTL_REG                      COMPCTL;                      
    union   COMPHYSCTL_REG                   COMPHYSCTL;                   
    union   COMPSTS_REG                      COMPSTS;                      
    union   COMPSTSCLR_REG                   COMPSTSCLR;                   
    union   COMPDACCTL_REG                   COMPDACCTL;                   
    Uint16                                   rsvd1;                        
    union   DACHVALS_REG                     DACHVALS;                     
    union   DACHVALA_REG                     DACHVALA;                     
    Uint16                                   RAMPMAXREFA;                  
    Uint16                                   rsvd2;                        
    Uint16                                   RAMPMAXREFS;                  
    Uint16                                   rsvd3;                        
    Uint16                                   RAMPDECVALA;                  
    Uint16                                   rsvd4;                        
    Uint16                                   RAMPDECVALS;                  
    Uint16                                   rsvd5;                        
    Uint16                                   RAMPSTS;                      
    Uint16                                   rsvd6;                        
    union   DACLVALS_REG                     DACLVALS;                     
    union   DACLVALA_REG                     DACLVALA;                     
    union   RAMPDLYA_REG                     RAMPDLYA;                     
    union   RAMPDLYS_REG                     RAMPDLYS;                     
    union   CTRIPLFILCTL_REG                 CTRIPLFILCTL;                 
    Uint16                                   CTRIPLFILCLKCTL;              
    union   CTRIPHFILCTL_REG                 CTRIPHFILCTL;                 
    Uint16                                   CTRIPHFILCLKCTL;              
    union   COMPLOCK_REG                     COMPLOCK;                     
};




extern volatile struct CMPSS_REGS Cmpss1Regs;
extern volatile struct CMPSS_REGS Cmpss2Regs;
extern volatile struct CMPSS_REGS Cmpss3Regs;
extern volatile struct CMPSS_REGS Cmpss4Regs;



















































struct TIM_BITS {                       
    Uint16 LSW:16;                      
    Uint16 MSW:16;                      
};

union TIM_REG {
    Uint32  all;
    struct  TIM_BITS  bit;
};

struct PRD_BITS {                       
    Uint16 LSW:16;                      
    Uint16 MSW:16;                      
};

union PRD_REG {
    Uint32  all;
    struct  PRD_BITS  bit;
};

struct TCR_BITS {                       
    Uint16 rsvd1:4;                     
    Uint16 TSS:1;                       
    Uint16 TRB:1;                       
    Uint16 rsvd2:4;                     
    Uint16 SOFT:1;                      
    Uint16 FREE:1;                      
    Uint16 rsvd3:2;                     
    Uint16 TIE:1;                       
    Uint16 TIF:1;                       
};

union TCR_REG {
    Uint16  all;
    struct  TCR_BITS  bit;
};

struct TPR_BITS {                       
    Uint16 TDDR:8;                      
    Uint16 PSC:8;                       
};

union TPR_REG {
    Uint16  all;
    struct  TPR_BITS  bit;
};

struct TPRH_BITS {                      
    Uint16 TDDRH:8;                     
    Uint16 PSCH:8;                      
};

union TPRH_REG {
    Uint16  all;
    struct  TPRH_BITS  bit;
};

struct CPUTIMER_REGS {
    union   TIM_REG                          TIM;                          
    union   PRD_REG                          PRD;                          
    union   TCR_REG                          TCR;                          
    Uint16                                   rsvd1;                        
    union   TPR_REG                          TPR;                          
    union   TPRH_REG                         TPRH;                         
};




extern volatile struct CPUTIMER_REGS CpuTimer0Regs;
extern volatile struct CPUTIMER_REGS CpuTimer1Regs;
extern volatile struct CPUTIMER_REGS CpuTimer2Regs;



















































struct DACREV_BITS {                    
    Uint16 REV:8;                       
    Uint16 rsvd1:8;                     
};

union DACREV_REG {
    Uint16  all;
    struct  DACREV_BITS  bit;
};

struct DACCTL_BITS {                    
    Uint16 DACREFSEL:1;                 
    Uint16 MODE:1;                      
    Uint16 LOADMODE:1;                  
    Uint16 rsvd1:1;                     
    Uint16 SYNCSEL:4;                   
    Uint16 rsvd2:8;                     
};

union DACCTL_REG {
    Uint16  all;
    struct  DACCTL_BITS  bit;
};

struct DACVALA_BITS {                   
    Uint16 DACVALA:12;                  
    Uint16 rsvd1:4;                     
};

union DACVALA_REG {
    Uint16  all;
    struct  DACVALA_BITS  bit;
};

struct DACVALS_BITS {                   
    Uint16 DACVALS:12;                  
    Uint16 rsvd1:4;                     
};

union DACVALS_REG {
    Uint16  all;
    struct  DACVALS_BITS  bit;
};

struct DACOUTEN_BITS {                  
    Uint16 DACOUTEN:1;                  
    Uint16 rsvd1:15;                    
};

union DACOUTEN_REG {
    Uint16  all;
    struct  DACOUTEN_BITS  bit;
};

struct DACLOCK_BITS {                   
    Uint16 DACCTL:1;                    
    Uint16 DACVAL:1;                    
    Uint16 DACOUTEN:1;                  
    Uint16 rsvd1:9;                     
    Uint16 KEY:4;                       
};

union DACLOCK_REG {
    Uint16  all;
    struct  DACLOCK_BITS  bit;
};

struct DACTRIM_BITS {                   
    Uint16 OFFSET_TRIM:8;               
    Uint16 rsvd1:4;                     
    Uint16 rsvd2:4;                     
};

union DACTRIM_REG {
    Uint16  all;
    struct  DACTRIM_BITS  bit;
};

struct DAC_REGS {
    union   DACREV_REG                       DACREV;                       
    union   DACCTL_REG                       DACCTL;                       
    union   DACVALA_REG                      DACVALA;                      
    union   DACVALS_REG                      DACVALS;                      
    union   DACOUTEN_REG                     DACOUTEN;                     
    union   DACLOCK_REG                      DACLOCK;                      
    union   DACTRIM_REG                      DACTRIM;                      
};




extern volatile struct DAC_REGS DacaRegs;
extern volatile struct DAC_REGS DacbRegs;



















































struct DCSM_Z1_OTP {
    Uint32                                   Z1OTP_LINKPOINTER1;           
    Uint32                                   Z1OTP_LINKPOINTER2;           
    Uint32                                   Z1OTP_LINKPOINTER3;           
    Uint32                                   Z1OTP_JLM_ENABLE;             
    Uint32                                   Z1OTP_GPREG1;                 
    Uint32                                   Z1OTP_GPREG2;                 
    Uint32                                   Z1OTP_GPREG3;                 
    Uint32                                   Z1OTP_GPREG4;                 
    Uint32                                   Z1OTP_PSWDLOCK;               
    Uint32                                   Z1OTP_CRCLOCK;                
    Uint32                                   Z1OTP_JTAGPSWDH0;             
    Uint32                                   Z1OTP_JTAGPSWDH1;             
    Uint32                                   Z1OTP_CMACKEY0;               
    Uint32                                   Z1OTP_CMACKEY1;               
    Uint32                                   Z1OTP_CMACKEY2;               
    Uint32                                   Z1OTP_CMACKEY3;               
};

struct DCSM_Z2_OTP {
    Uint32                                   Z2OTP_LINKPOINTER1;           
    Uint32                                   Z2OTP_LINKPOINTER2;           
    Uint32                                   Z2OTP_LINKPOINTER3;           
    Uint16                                   rsvd1[2];                     
    Uint32                                   Z2OTP_GPREG1;                 
    Uint32                                   Z2OTP_GPREG2;                 
    Uint32                                   Z2OTP_GPREG3;                 
    Uint32                                   Z2OTP_GPREG4;                 
    Uint32                                   Z2OTP_PSWDLOCK;               
    Uint32                                   Z2OTP_CRCLOCK;                
};

struct Z1_LINKPOINTER_BITS {            
    Uint16 LINKPOINTER:14;              
    Uint16 rsvd1:2;                     
    Uint16 rsvd2:16;                    
};

union Z1_LINKPOINTER_REG {
    Uint32  all;
    struct  Z1_LINKPOINTER_BITS  bit;
};

struct Z1_OTPSECLOCK_BITS {             
    Uint16 JTAGLOCK:1;                  
    Uint16 rsvd1:3;                     
    Uint16 PSWDLOCK:4;                  
    Uint16 CRCLOCK:4;                   
    Uint16 rsvd2:4;                     
    Uint16 rsvd3:16;                    
};

union Z1_OTPSECLOCK_REG {
    Uint32  all;
    struct  Z1_OTPSECLOCK_BITS  bit;
};

struct Z1_JLM_ENABLE_BITS {             
    Uint16 Z1_JLM_ENABLE:4;             
    Uint16 rsvd1:12;                    
    Uint16 rsvd2:16;                    
};

union Z1_JLM_ENABLE_REG {
    Uint32  all;
    struct  Z1_JLM_ENABLE_BITS  bit;
};

struct Z1_LINKPOINTERERR_BITS {         
    Uint16 Z1_LINKPOINTERERR:14;        
    Uint16 rsvd1:2;                     
    Uint16 rsvd2:16;                    
};

union Z1_LINKPOINTERERR_REG {
    Uint32  all;
    struct  Z1_LINKPOINTERERR_BITS  bit;
};

struct Z1_CR_BITS {                     
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:12;                    
    Uint16 rsvd6:3;                     
    Uint16 ALLZERO:1;                   
    Uint16 ALLONE:1;                    
    Uint16 UNSECURE:1;                  
    Uint16 ARMED:1;                     
    Uint16 rsvd7:1;                     
    Uint16 rsvd8:7;                     
    Uint16 FORCESEC:1;                  
};

union Z1_CR_REG {
    Uint32  all;
    struct  Z1_CR_BITS  bit;
};

struct Z1_GRABSECT1R_BITS {             
    Uint16 GRAB_SECT0:2;                
    Uint16 GRAB_SECT1:2;                
    Uint16 GRAB_SECT2:2;                
    Uint16 GRAB_SECT3:2;                
    Uint16 GRAB_SECT4:2;                
    Uint16 GRAB_SECT5:2;                
    Uint16 GRAB_SECT6:2;                
    Uint16 GRAB_SECT7:2;                
    Uint16 GRAB_SECT8:2;                
    Uint16 GRAB_SECT9:2;                
    Uint16 GRAB_SECT10:2;               
    Uint16 GRAB_SECT11:2;               
    Uint16 GRAB_SECT12:2;               
    Uint16 GRAB_SECT13:2;               
    Uint16 GRAB_SECT14:2;               
    Uint16 GRAB_SECT15:2;               
};

union Z1_GRABSECT1R_REG {
    Uint32  all;
    struct  Z1_GRABSECT1R_BITS  bit;
};

struct Z1_GRABSECT2R_BITS {             
    Uint16 GRAB_SECT0:2;                
    Uint16 GRAB_SECT1:2;                
    Uint16 GRAB_SECT2:2;                
    Uint16 GRAB_SECT3:2;                
    Uint16 GRAB_SECT4:2;                
    Uint16 GRAB_SECT5:2;                
    Uint16 GRAB_SECT6:2;                
    Uint16 GRAB_SECT7:2;                
    Uint16 GRAB_SECT8:2;                
    Uint16 GRAB_SECT9:2;                
    Uint16 GRAB_SECT10:2;               
    Uint16 GRAB_SECT11:2;               
    Uint16 GRAB_SECT12:2;               
    Uint16 GRAB_SECT13:2;               
    Uint16 GRAB_SECT14:2;               
    Uint16 GRAB_SECT15:2;               
};

union Z1_GRABSECT2R_REG {
    Uint32  all;
    struct  Z1_GRABSECT2R_BITS  bit;
};

struct Z1_GRABSECT3R_BITS {             
    Uint16 GRAB_SECT0:2;                
    Uint16 GRAB_SECT1:2;                
    Uint16 GRAB_SECT2:2;                
    Uint16 GRAB_SECT3:2;                
    Uint16 GRAB_SECT4:2;                
    Uint16 GRAB_SECT5:2;                
    Uint16 GRAB_SECT6:2;                
    Uint16 GRAB_SECT7:2;                
    Uint16 GRAB_SECT8:2;                
    Uint16 GRAB_SECT9:2;                
    Uint16 GRAB_SECT10:2;               
    Uint16 GRAB_SECT11:2;               
    Uint16 GRAB_SECT12:2;               
    Uint16 GRAB_SECT13:2;               
    Uint16 GRAB_SECT14:2;               
    Uint16 GRAB_SECT15:2;               
};

union Z1_GRABSECT3R_REG {
    Uint32  all;
    struct  Z1_GRABSECT3R_BITS  bit;
};

struct Z1_GRABRAM1R_BITS {              
    Uint16 GRAB_RAM0:2;                 
    Uint16 GRAB_RAM1:2;                 
    Uint16 GRAB_RAM2:2;                 
    Uint16 GRAB_RAM3:2;                 
    Uint16 GRAB_RAM4:2;                 
    Uint16 GRAB_RAM5:2;                 
    Uint16 GRAB_RAM6:2;                 
    Uint16 GRAB_RAM7:2;                 
    Uint16 rsvd1:16;                    
};

union Z1_GRABRAM1R_REG {
    Uint32  all;
    struct  Z1_GRABRAM1R_BITS  bit;
};

struct Z1_EXEONLYSECT1R_BITS {          
    Uint16 EXEONLY_BANK0_SECT0:1;       
    Uint16 EXEONLY_BANK0_SECT1:1;       
    Uint16 EXEONLY_BANK0_SECT2:1;       
    Uint16 EXEONLY_BANK0_SECT3:1;       
    Uint16 EXEONLY_BANK0_SECT4:1;       
    Uint16 EXEONLY_BANK0_SECT5:1;       
    Uint16 EXEONLY_BANK0_SECT6:1;       
    Uint16 EXEONLY_BANK0_SECT7:1;       
    Uint16 EXEONLY_BANK0_SECT8:1;       
    Uint16 EXEONLY_BANK0_SECT9:1;       
    Uint16 EXEONLY_BANK0_SECT10:1;      
    Uint16 EXEONLY_BANK0_SECT11:1;      
    Uint16 EXEONLY_BANK0_SECT12:1;      
    Uint16 EXEONLY_BANK0_SECT13:1;      
    Uint16 EXEONLY_BANK0_SECT14:1;      
    Uint16 EXEONLY_BANK0_SECT15:1;      
    Uint16 EXEONLY_BANK1_SECT0:1;       
    Uint16 EXEONLY_BANK1_SECT1:1;       
    Uint16 EXEONLY_BANK1_SECT2:1;       
    Uint16 EXEONLY_BANK1_SECT3:1;       
    Uint16 EXEONLY_BANK1_SECT4:1;       
    Uint16 EXEONLY_BANK1_SECT5:1;       
    Uint16 EXEONLY_BANK1_SECT6:1;       
    Uint16 EXEONLY_BANK1_SECT7:1;       
    Uint16 EXEONLY_BANK1_SECT8:1;       
    Uint16 EXEONLY_BANK1_SECT9:1;       
    Uint16 EXEONLY_BANK1_SECT10:1;      
    Uint16 EXEONLY_BANK1_SECT11:1;      
    Uint16 EXEONLY_BANK1_SECT12:1;      
    Uint16 EXEONLY_BANK1_SECT13:1;      
    Uint16 EXEONLY_BANK1_SECT14:1;      
    Uint16 EXEONLY_BANK1_SECT15:1;      
};

union Z1_EXEONLYSECT1R_REG {
    Uint32  all;
    struct  Z1_EXEONLYSECT1R_BITS  bit;
};

struct Z1_EXEONLYSECT2R_BITS {          
    Uint16 EXEONLY_BANK2_SECT0:1;       
    Uint16 EXEONLY_BANK2_SECT1:1;       
    Uint16 EXEONLY_BANK2_SECT2:1;       
    Uint16 EXEONLY_BANK2_SECT3:1;       
    Uint16 EXEONLY_BANK2_SECT4:1;       
    Uint16 EXEONLY_BANK2_SECT5:1;       
    Uint16 EXEONLY_BANK2_SECT6:1;       
    Uint16 EXEONLY_BANK2_SECT7:1;       
    Uint16 EXEONLY_BANK2_SECT8:1;       
    Uint16 EXEONLY_BANK2_SECT9:1;       
    Uint16 EXEONLY_BANK2_SECT10:1;      
    Uint16 EXEONLY_BANK2_SECT11:1;      
    Uint16 EXEONLY_BANK2_SECT12:1;      
    Uint16 EXEONLY_BANK2_SECT13:1;      
    Uint16 EXEONLY_BANK2_SECT14:1;      
    Uint16 EXEONLY_BANK2_SECT15:1;      
    Uint16 rsvd1:16;                    
};

union Z1_EXEONLYSECT2R_REG {
    Uint32  all;
    struct  Z1_EXEONLYSECT2R_BITS  bit;
};

struct Z1_EXEONLYRAM1R_BITS {           
    Uint16 EXEONLY_RAM0:1;              
    Uint16 EXEONLY_RAM1:1;              
    Uint16 EXEONLY_RAM2:1;              
    Uint16 EXEONLY_RAM3:1;              
    Uint16 EXEONLY_RAM4:1;              
    Uint16 EXEONLY_RAM5:1;              
    Uint16 EXEONLY_RAM6:1;              
    Uint16 EXEONLY_RAM7:1;              
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union Z1_EXEONLYRAM1R_REG {
    Uint32  all;
    struct  Z1_EXEONLYRAM1R_BITS  bit;
};

struct DCSM_Z1_REGS {
    union   Z1_LINKPOINTER_REG               Z1_LINKPOINTER;               
    union   Z1_OTPSECLOCK_REG                Z1_OTPSECLOCK;                
    union   Z1_JLM_ENABLE_REG                Z1_JLM_ENABLE;                
    union   Z1_LINKPOINTERERR_REG            Z1_LINKPOINTERERR;            
    Uint32                                   Z1_GPREG1;                    
    Uint32                                   Z1_GPREG2;                    
    Uint32                                   Z1_GPREG3;                    
    Uint32                                   Z1_GPREG4;                    
    Uint32                                   Z1_CSMKEY0;                   
    Uint32                                   Z1_CSMKEY1;                   
    Uint32                                   Z1_CSMKEY2;                   
    Uint32                                   Z1_CSMKEY3;                   
    union   Z1_CR_REG                        Z1_CR;                        
    union   Z1_GRABSECT1R_REG                Z1_GRABSECT1R;                
    union   Z1_GRABSECT2R_REG                Z1_GRABSECT2R;                
    union   Z1_GRABSECT3R_REG                Z1_GRABSECT3R;                
    union   Z1_GRABRAM1R_REG                 Z1_GRABRAM1R;                 
    Uint16                                   rsvd1[4];                     
    union   Z1_EXEONLYSECT1R_REG             Z1_EXEONLYSECT1R;             
    union   Z1_EXEONLYSECT2R_REG             Z1_EXEONLYSECT2R;             
    union   Z1_EXEONLYRAM1R_REG              Z1_EXEONLYRAM1R;              
    Uint16                                   rsvd2[2];                     
    Uint32                                   Z1_JTAGKEY0;                  
    Uint32                                   Z1_JTAGKEY1;                  
    Uint32                                   Z1_JTAGKEY2;                  
    Uint32                                   Z1_JTAGKEY3;                  
    Uint32                                   Z1_CMACKEY0;                  
    Uint32                                   Z1_CMACKEY1;                  
    Uint32                                   Z1_CMACKEY2;                  
    Uint32                                   Z1_CMACKEY3;                  
};

struct Z2_LINKPOINTER_BITS {            
    Uint16 LINKPOINTER:14;              
    Uint16 rsvd1:2;                     
    Uint16 rsvd2:16;                    
};

union Z2_LINKPOINTER_REG {
    Uint32  all;
    struct  Z2_LINKPOINTER_BITS  bit;
};

struct Z2_OTPSECLOCK_BITS {             
    Uint16 JTAGLOCK:1;                  
    Uint16 rsvd1:3;                     
    Uint16 PSWDLOCK:4;                  
    Uint16 CRCLOCK:4;                   
    Uint16 rsvd2:4;                     
    Uint16 rsvd3:16;                    
};

union Z2_OTPSECLOCK_REG {
    Uint32  all;
    struct  Z2_OTPSECLOCK_BITS  bit;
};

struct Z2_LINKPOINTERERR_BITS {         
    Uint16 Z2_LINKPOINTERERR:14;        
    Uint16 rsvd1:2;                     
    Uint16 rsvd2:16;                    
};

union Z2_LINKPOINTERERR_REG {
    Uint32  all;
    struct  Z2_LINKPOINTERERR_BITS  bit;
};

struct Z2_CR_BITS {                     
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:12;                    
    Uint16 rsvd6:3;                     
    Uint16 ALLZERO:1;                   
    Uint16 ALLONE:1;                    
    Uint16 UNSECURE:1;                  
    Uint16 ARMED:1;                     
    Uint16 rsvd7:1;                     
    Uint16 rsvd8:7;                     
    Uint16 FORCESEC:1;                  
};

union Z2_CR_REG {
    Uint32  all;
    struct  Z2_CR_BITS  bit;
};

struct Z2_GRABSECT1R_BITS {             
    Uint16 GRAB_SECT0:2;                
    Uint16 GRAB_SECT1:2;                
    Uint16 GRAB_SECT2:2;                
    Uint16 GRAB_SECT3:2;                
    Uint16 GRAB_SECT4:2;                
    Uint16 GRAB_SECT5:2;                
    Uint16 GRAB_SECT6:2;                
    Uint16 GRAB_SECT7:2;                
    Uint16 GRAB_SECT8:2;                
    Uint16 GRAB_SECT9:2;                
    Uint16 GRAB_SECT10:2;               
    Uint16 GRAB_SECT11:2;               
    Uint16 GRAB_SECT12:2;               
    Uint16 GRAB_SECT13:2;               
    Uint16 GRAB_SECT14:2;               
    Uint16 GRAB_SECT15:2;               
};

union Z2_GRABSECT1R_REG {
    Uint32  all;
    struct  Z2_GRABSECT1R_BITS  bit;
};

struct Z2_GRABSECT2R_BITS {             
    Uint16 GRAB_SECT0:2;                
    Uint16 GRAB_SECT1:2;                
    Uint16 GRAB_SECT2:2;                
    Uint16 GRAB_SECT3:2;                
    Uint16 GRAB_SECT4:2;                
    Uint16 GRAB_SECT5:2;                
    Uint16 GRAB_SECT6:2;                
    Uint16 GRAB_SECT7:2;                
    Uint16 GRAB_SECT8:2;                
    Uint16 GRAB_SECT9:2;                
    Uint16 GRAB_SECT10:2;               
    Uint16 GRAB_SECT11:2;               
    Uint16 GRAB_SECT12:2;               
    Uint16 GRAB_SECT13:2;               
    Uint16 GRAB_SECT14:2;               
    Uint16 GRAB_SECT15:2;               
};

union Z2_GRABSECT2R_REG {
    Uint32  all;
    struct  Z2_GRABSECT2R_BITS  bit;
};

struct Z2_GRABSECT3R_BITS {             
    Uint16 GRAB_SECT0:2;                
    Uint16 GRAB_SECT1:2;                
    Uint16 GRAB_SECT2:2;                
    Uint16 GRAB_SECT3:2;                
    Uint16 GRAB_SECT4:2;                
    Uint16 GRAB_SECT5:2;                
    Uint16 GRAB_SECT6:2;                
    Uint16 GRAB_SECT7:2;                
    Uint16 GRAB_SECT8:2;                
    Uint16 GRAB_SECT9:2;                
    Uint16 GRAB_SECT10:2;               
    Uint16 GRAB_SECT11:2;               
    Uint16 GRAB_SECT12:2;               
    Uint16 GRAB_SECT13:2;               
    Uint16 GRAB_SECT14:2;               
    Uint16 GRAB_SECT15:2;               
};

union Z2_GRABSECT3R_REG {
    Uint32  all;
    struct  Z2_GRABSECT3R_BITS  bit;
};

struct Z2_GRABRAM1R_BITS {              
    Uint16 GRAB_RAM0:2;                 
    Uint16 GRAB_RAM1:2;                 
    Uint16 GRAB_RAM2:2;                 
    Uint16 GRAB_RAM3:2;                 
    Uint16 GRAB_RAM4:2;                 
    Uint16 GRAB_RAM5:2;                 
    Uint16 GRAB_RAM6:2;                 
    Uint16 GRAB_RAM7:2;                 
    Uint16 rsvd1:16;                    
};

union Z2_GRABRAM1R_REG {
    Uint32  all;
    struct  Z2_GRABRAM1R_BITS  bit;
};

struct Z2_EXEONLYSECT1R_BITS {          
    Uint16 EXEONLY_BANK0_SECT0:1;       
    Uint16 EXEONLY_BANK0_SECT1:1;       
    Uint16 EXEONLY_BANK0_SECT2:1;       
    Uint16 EXEONLY_BANK0_SECT3:1;       
    Uint16 EXEONLY_BANK0_SECT4:1;       
    Uint16 EXEONLY_BANK0_SECT5:1;       
    Uint16 EXEONLY_BANK0_SECT6:1;       
    Uint16 EXEONLY_BANK0_SECT7:1;       
    Uint16 EXEONLY_BANK0_SECT8:1;       
    Uint16 EXEONLY_BANK0_SECT9:1;       
    Uint16 EXEONLY_BANK0_SECT10:1;      
    Uint16 EXEONLY_BANK0_SECT11:1;      
    Uint16 EXEONLY_BANK0_SECT12:1;      
    Uint16 EXEONLY_BANK0_SECT13:1;      
    Uint16 EXEONLY_BANK0_SECT14:1;      
    Uint16 EXEONLY_BANK0_SECT15:1;      
    Uint16 EXEONLY_BANK1_SECT0:1;       
    Uint16 EXEONLY_BANK1_SECT1:1;       
    Uint16 EXEONLY_BANK1_SECT2:1;       
    Uint16 EXEONLY_BANK1_SECT3:1;       
    Uint16 EXEONLY_BANK1_SECT4:1;       
    Uint16 EXEONLY_BANK1_SECT5:1;       
    Uint16 EXEONLY_BANK1_SECT6:1;       
    Uint16 EXEONLY_BANK1_SECT7:1;       
    Uint16 EXEONLY_BANK1_SECT8:1;       
    Uint16 EXEONLY_BANK1_SECT9:1;       
    Uint16 EXEONLY_BANK1_SECT10:1;      
    Uint16 EXEONLY_BANK1_SECT11:1;      
    Uint16 EXEONLY_BANK1_SECT12:1;      
    Uint16 EXEONLY_BANK1_SECT13:1;      
    Uint16 EXEONLY_BANK1_SECT14:1;      
    Uint16 EXEONLY_BANK1_SECT15:1;      
};

union Z2_EXEONLYSECT1R_REG {
    Uint32  all;
    struct  Z2_EXEONLYSECT1R_BITS  bit;
};

struct Z2_EXEONLYSECT2R_BITS {          
    Uint16 EXEONLY_BANK2_SECT0:1;       
    Uint16 EXEONLY_BANK2_SECT1:1;       
    Uint16 EXEONLY_BANK2_SECT2:1;       
    Uint16 EXEONLY_BANK2_SECT3:1;       
    Uint16 EXEONLY_BANK2_SECT4:1;       
    Uint16 EXEONLY_BANK2_SECT5:1;       
    Uint16 EXEONLY_BANK2_SECT6:1;       
    Uint16 EXEONLY_BANK2_SECT7:1;       
    Uint16 EXEONLY_BANK2_SECT8:1;       
    Uint16 EXEONLY_BANK2_SECT9:1;       
    Uint16 EXEONLY_BANK2_SECT10:1;      
    Uint16 EXEONLY_BANK2_SECT11:1;      
    Uint16 EXEONLY_BANK2_SECT12:1;      
    Uint16 EXEONLY_BANK2_SECT13:1;      
    Uint16 EXEONLY_BANK2_SECT14:1;      
    Uint16 EXEONLY_BANK2_SECT15:1;      
    Uint16 rsvd1:16;                    
};

union Z2_EXEONLYSECT2R_REG {
    Uint32  all;
    struct  Z2_EXEONLYSECT2R_BITS  bit;
};

struct Z2_EXEONLYRAM1R_BITS {           
    Uint16 EXEONLY_RAM0:1;              
    Uint16 EXEONLY_RAM1:1;              
    Uint16 EXEONLY_RAM2:1;              
    Uint16 EXEONLY_RAM3:1;              
    Uint16 EXEONLY_RAM4:1;              
    Uint16 EXEONLY_RAM5:1;              
    Uint16 EXEONLY_RAM6:1;              
    Uint16 EXEONLY_RAM7:1;              
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union Z2_EXEONLYRAM1R_REG {
    Uint32  all;
    struct  Z2_EXEONLYRAM1R_BITS  bit;
};

struct DCSM_Z2_REGS {
    union   Z2_LINKPOINTER_REG               Z2_LINKPOINTER;               
    union   Z2_OTPSECLOCK_REG                Z2_OTPSECLOCK;                
    Uint16                                   rsvd1[2];                     
    union   Z2_LINKPOINTERERR_REG            Z2_LINKPOINTERERR;            
    Uint32                                   Z2_GPREG1;                    
    Uint32                                   Z2_GPREG2;                    
    Uint32                                   Z2_GPREG3;                    
    Uint32                                   Z2_GPREG4;                    
    Uint32                                   Z2_CSMKEY0;                   
    Uint32                                   Z2_CSMKEY1;                   
    Uint32                                   Z2_CSMKEY2;                   
    Uint32                                   Z2_CSMKEY3;                   
    union   Z2_CR_REG                        Z2_CR;                        
    union   Z2_GRABSECT1R_REG                Z2_GRABSECT1R;                
    union   Z2_GRABSECT2R_REG                Z2_GRABSECT2R;                
    union   Z2_GRABSECT3R_REG                Z2_GRABSECT3R;                
    union   Z2_GRABRAM1R_REG                 Z2_GRABRAM1R;                 
    Uint16                                   rsvd2[4];                     
    union   Z2_EXEONLYSECT1R_REG             Z2_EXEONLYSECT1R;             
    union   Z2_EXEONLYSECT2R_REG             Z2_EXEONLYSECT2R;             
    union   Z2_EXEONLYRAM1R_REG              Z2_EXEONLYRAM1R;              
};

struct FLSEM_BITS {                     
    Uint16 SEM:2;                       
    Uint16 rsvd1:6;                     
    Uint16 KEY:8;                       
    Uint16 rsvd2:16;                    
};

union FLSEM_REG {
    Uint32  all;
    struct  FLSEM_BITS  bit;
};

struct SECTSTAT1_BITS {                 
    Uint16 STATUS_SECT0:2;              
    Uint16 STATUS_SECT1:2;              
    Uint16 STATUS_SECT2:2;              
    Uint16 STATUS_SECT3:2;              
    Uint16 STATUS_SECT4:2;              
    Uint16 STATUS_SECT5:2;              
    Uint16 STATUS_SECT6:2;              
    Uint16 STATUS_SECT7:2;              
    Uint16 STATUS_SECT8:2;              
    Uint16 STATUS_SECT9:2;              
    Uint16 STATUS_SECT10:2;             
    Uint16 STATUS_SECT11:2;             
    Uint16 STATUS_SECT12:2;             
    Uint16 STATUS_SECT13:2;             
    Uint16 STATUS_SECT14:2;             
    Uint16 STATUS_SECT15:2;             
};

union SECTSTAT1_REG {
    Uint32  all;
    struct  SECTSTAT1_BITS  bit;
};

struct SECTSTAT2_BITS {                 
    Uint16 STATUS_SECT0:2;              
    Uint16 STATUS_SECT1:2;              
    Uint16 STATUS_SECT2:2;              
    Uint16 STATUS_SECT3:2;              
    Uint16 STATUS_SECT4:2;              
    Uint16 STATUS_SECT5:2;              
    Uint16 STATUS_SECT6:2;              
    Uint16 STATUS_SECT7:2;              
    Uint16 STATUS_SECT8:2;              
    Uint16 STATUS_SECT9:2;              
    Uint16 STATUS_SECT10:2;             
    Uint16 STATUS_SECT11:2;             
    Uint16 STATUS_SECT12:2;             
    Uint16 STATUS_SECT13:2;             
    Uint16 STATUS_SECT14:2;             
    Uint16 STATUS_SECT15:2;             
};

union SECTSTAT2_REG {
    Uint32  all;
    struct  SECTSTAT2_BITS  bit;
};

struct SECTSTAT3_BITS {                 
    Uint16 STATUS_SECT0:2;              
    Uint16 STATUS_SECT1:2;              
    Uint16 STATUS_SECT2:2;              
    Uint16 STATUS_SECT3:2;              
    Uint16 STATUS_SECT4:2;              
    Uint16 STATUS_SECT5:2;              
    Uint16 STATUS_SECT6:2;              
    Uint16 STATUS_SECT7:2;              
    Uint16 STATUS_SECT8:2;              
    Uint16 STATUS_SECT9:2;              
    Uint16 STATUS_SECT10:2;             
    Uint16 STATUS_SECT11:2;             
    Uint16 STATUS_SECT12:2;             
    Uint16 STATUS_SECT13:2;             
    Uint16 STATUS_SECT14:2;             
    Uint16 STATUS_SECT15:2;             
};

union SECTSTAT3_REG {
    Uint32  all;
    struct  SECTSTAT3_BITS  bit;
};

struct RAMSTAT1_BITS {                  
    Uint16 STATUS_RAM0:2;               
    Uint16 STATUS_RAM1:2;               
    Uint16 STATUS_RAM2:2;               
    Uint16 STATUS_RAM3:2;               
    Uint16 STATUS_RAM4:2;               
    Uint16 STATUS_RAM5:2;               
    Uint16 STATUS_RAM6:2;               
    Uint16 STATUS_RAM7:2;               
    Uint16 rsvd1:16;                    
};

union RAMSTAT1_REG {
    Uint32  all;
    struct  RAMSTAT1_BITS  bit;
};

struct SECERRSTAT_BITS {                
    Uint16 ERR:1;                       
    Uint16 rsvd1:15;                    
    Uint16 rsvd2:16;                    
};

union SECERRSTAT_REG {
    Uint32  all;
    struct  SECERRSTAT_BITS  bit;
};

struct SECERRCLR_BITS {                 
    Uint16 ERR:1;                       
    Uint16 rsvd1:15;                    
    Uint16 rsvd2:16;                    
};

union SECERRCLR_REG {
    Uint32  all;
    struct  SECERRCLR_BITS  bit;
};

struct SECERRFRC_BITS {                 
    Uint16 ERR:1;                       
    Uint16 rsvd1:15;                    
    Uint16 KEY:16;                      
};

union SECERRFRC_REG {
    Uint32  all;
    struct  SECERRFRC_BITS  bit;
};

struct DCSM_COMMON_REGS {
    union   FLSEM_REG                        FLSEM;                        
    Uint16                                   rsvd1[6];                     
    union   SECTSTAT1_REG                    SECTSTAT1;                    
    union   SECTSTAT2_REG                    SECTSTAT2;                    
    union   SECTSTAT3_REG                    SECTSTAT3;                    
    Uint16                                   rsvd2[2];                     
    union   RAMSTAT1_REG                     RAMSTAT1;                     
    Uint16                                   rsvd3[6];                     
    union   SECERRSTAT_REG                   SECERRSTAT;                   
    union   SECERRCLR_REG                    SECERRCLR;                    
    union   SECERRFRC_REG                    SECERRFRC;                    
};




extern volatile struct DCSM_Z1_REGS DcsmZ1Regs;
extern volatile struct DCSM_Z2_REGS DcsmZ2Regs;
extern volatile struct DCSM_COMMON_REGS DcsmCommonRegs;
extern volatile struct DCSM_Z1_OTP DcsmZ1OtpRegs;
extern volatile struct DCSM_Z2_OTP DcsmZ2OtpRegs;



















































struct MODE_BITS {                      
    Uint16 PERINTSEL:5;                 
    Uint16 rsvd1:2;                     
    Uint16 OVRINTE:1;                   
    Uint16 PERINTE:1;                   
    Uint16 CHINTMODE:1;                 
    Uint16 ONESHOT:1;                   
    Uint16 CONTINUOUS:1;                
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 DATASIZE:1;                  
    Uint16 CHINTE:1;                    
};

union MODE_REG {
    Uint16  all;
    struct  MODE_BITS  bit;
};

struct CONTROL_BITS {                   
    Uint16 RUN:1;                       
    Uint16 HALT:1;                      
    Uint16 SOFTRESET:1;                 
    Uint16 PERINTFRC:1;                 
    Uint16 PERINTCLR:1;                 
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 ERRCLR:1;                    
    Uint16 PERINTFLG:1;                 
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 TRANSFERSTS:1;               
    Uint16 BURSTSTS:1;                  
    Uint16 RUNSTS:1;                    
    Uint16 OVRFLG:1;                    
    Uint16 rsvd5:1;                     
};

union CONTROL_REG {
    Uint16  all;
    struct  CONTROL_BITS  bit;
};

struct BURST_SIZE_BITS {                
    Uint16 BURSTSIZE:5;                 
    Uint16 rsvd1:11;                    
};

union BURST_SIZE_REG {
    Uint16  all;
    struct  BURST_SIZE_BITS  bit;
};

struct BURST_COUNT_BITS {               
    Uint16 BURSTCOUNT:5;                
    Uint16 rsvd1:11;                    
};

union BURST_COUNT_REG {
    Uint16  all;
    struct  BURST_COUNT_BITS  bit;
};

struct CH_REGS {
    union   MODE_REG                         MODE;                         
    union   CONTROL_REG                      CONTROL;                      
    union   BURST_SIZE_REG                   BURST_SIZE;                   
    union   BURST_COUNT_REG                  BURST_COUNT;                  
    int16                                    SRC_BURST_STEP;               
    int16                                    DST_BURST_STEP;               
    Uint16                                   TRANSFER_SIZE;                
    Uint16                                   TRANSFER_COUNT;               
    int16                                    SRC_TRANSFER_STEP;            
    int16                                    DST_TRANSFER_STEP;            
    Uint16                                   SRC_WRAP_SIZE;                
    Uint16                                   SRC_WRAP_COUNT;               
    int16                                    SRC_WRAP_STEP;                
    Uint16                                   DST_WRAP_SIZE;                
    Uint16                                   DST_WRAP_COUNT;               
    int16                                    DST_WRAP_STEP;                
    Uint32                                   SRC_BEG_ADDR_SHADOW;          
    Uint32                                   SRC_ADDR_SHADOW;              
    Uint32                                   SRC_BEG_ADDR_ACTIVE;          
    Uint32                                   SRC_ADDR_ACTIVE;              
    Uint32                                   DST_BEG_ADDR_SHADOW;          
    Uint32                                   DST_ADDR_SHADOW;              
    Uint32                                   DST_BEG_ADDR_ACTIVE;          
    Uint32                                   DST_ADDR_ACTIVE;              
};

struct DMACTRL_BITS {                   
    Uint16 HARDRESET:1;                 
    Uint16 PRIORITYRESET:1;             
    Uint16 rsvd1:14;                    
};

union DMACTRL_REG {
    Uint16  all;
    struct  DMACTRL_BITS  bit;
};

struct DEBUGCTRL_BITS {                 
    Uint16 rsvd1:15;                    
    Uint16 FREE:1;                      
};

union DEBUGCTRL_REG {
    Uint16  all;
    struct  DEBUGCTRL_BITS  bit;
};

struct PRIORITYCTRL1_BITS {             
    Uint16 CH1PRIORITY:1;               
    Uint16 rsvd1:15;                    
};

union PRIORITYCTRL1_REG {
    Uint16  all;
    struct  PRIORITYCTRL1_BITS  bit;
};

struct PRIORITYSTAT_BITS {              
    Uint16 ACTIVESTS:3;                 
    Uint16 rsvd1:1;                     
    Uint16 ACTIVESTS_SHADOW:3;          
    Uint16 rsvd2:9;                     
};

union PRIORITYSTAT_REG {
    Uint16  all;
    struct  PRIORITYSTAT_BITS  bit;
};

struct DMA_REGS {
    union   DMACTRL_REG                      DMACTRL;                      
    union   DEBUGCTRL_REG                    DEBUGCTRL;                    
    Uint16                                   rsvd1[2];                     
    union   PRIORITYCTRL1_REG                PRIORITYCTRL1;                
    Uint16                                   rsvd2;                        
    union   PRIORITYSTAT_REG                 PRIORITYSTAT;                 
    Uint16                      rsvd3[25];      
    struct CH_REGS                 CH1;            
    struct CH_REGS                 CH2;            
    struct CH_REGS                 CH3;            
    struct CH_REGS                 CH4;            
    struct CH_REGS                 CH5;            
    struct CH_REGS                 CH6;            
};




extern volatile struct DMA_REGS DmaRegs;



















































struct ECCTL0_BITS {                    
    Uint16 INPUTSEL:7;                  
    Uint16 rsvd1:9;                     
    Uint16 rsvd2:16;                    
};

union ECCTL0_REG {
    Uint32  all;
    struct  ECCTL0_BITS  bit;
};

struct ECCTL1_BITS {                    
    Uint16 CAP1POL:1;                   
    Uint16 CTRRST1:1;                   
    Uint16 CAP2POL:1;                   
    Uint16 CTRRST2:1;                   
    Uint16 CAP3POL:1;                   
    Uint16 CTRRST3:1;                   
    Uint16 CAP4POL:1;                   
    Uint16 CTRRST4:1;                   
    Uint16 CAPLDEN:1;                   
    Uint16 PRESCALE:5;                  
    Uint16 FREE_SOFT:2;                 
};

union ECCTL1_REG {
    Uint16  all;
    struct  ECCTL1_BITS  bit;
};

struct ECCTL2_BITS {                    
    Uint16 CONT_ONESHT:1;               
    Uint16 STOP_WRAP:2;                 
    Uint16 REARM:1;                     
    Uint16 TSCTRSTOP:1;                 
    Uint16 SYNCI_EN:1;                  
    Uint16 SYNCO_SEL:2;                 
    Uint16 SWSYNC:1;                    
    Uint16 CAP_APWM:1;                  
    Uint16 APWMPOL:1;                   
    Uint16 CTRFILTRESET:1;              
    Uint16 DMAEVTSEL:2;                 
    Uint16 MODCNTRSTS:2;                
};

union ECCTL2_REG {
    Uint16  all;
    struct  ECCTL2_BITS  bit;
};

struct ECEINT_BITS {                    
    Uint16 rsvd1:1;                     
    Uint16 CEVT1:1;                     
    Uint16 CEVT2:1;                     
    Uint16 CEVT3:1;                     
    Uint16 CEVT4:1;                     
    Uint16 CTROVF:1;                    
    Uint16 CTR_EQ_PRD:1;                
    Uint16 CTR_EQ_CMP:1;                
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:7;                     
};

union ECEINT_REG {
    Uint16  all;
    struct  ECEINT_BITS  bit;
};

struct ECFLG_BITS {                     
    Uint16 INT:1;                       
    Uint16 CEVT1:1;                     
    Uint16 CEVT2:1;                     
    Uint16 CEVT3:1;                     
    Uint16 CEVT4:1;                     
    Uint16 CTROVF:1;                    
    Uint16 CTR_PRD:1;                   
    Uint16 CTR_CMP:1;                   
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:7;                     
};

union ECFLG_REG {
    Uint16  all;
    struct  ECFLG_BITS  bit;
};

struct ECCLR_BITS {                     
    Uint16 INT:1;                       
    Uint16 CEVT1:1;                     
    Uint16 CEVT2:1;                     
    Uint16 CEVT3:1;                     
    Uint16 CEVT4:1;                     
    Uint16 CTROVF:1;                    
    Uint16 CTR_PRD:1;                   
    Uint16 CTR_CMP:1;                   
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:7;                     
};

union ECCLR_REG {
    Uint16  all;
    struct  ECCLR_BITS  bit;
};

struct ECFRC_BITS {                     
    Uint16 rsvd1:1;                     
    Uint16 CEVT1:1;                     
    Uint16 CEVT2:1;                     
    Uint16 CEVT3:1;                     
    Uint16 CEVT4:1;                     
    Uint16 CTROVF:1;                    
    Uint16 CTR_PRD:1;                   
    Uint16 CTR_CMP:1;                   
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:7;                     
};

union ECFRC_REG {
    Uint16  all;
    struct  ECFRC_BITS  bit;
};

struct ECAPSYNCINSEL_BITS {             
    Uint16 SEL:5;                       
    Uint16 rsvd1:11;                    
    Uint16 rsvd2:16;                    
};

union ECAPSYNCINSEL_REG {
    Uint32  all;
    struct  ECAPSYNCINSEL_BITS  bit;
};

struct ECAP_REGS {
    Uint32                                   TSCTR;                        
    Uint32                                   CTRPHS;                       
    Uint32                                   CAP1;                         
    Uint32                                   CAP2;                         
    Uint32                                   CAP3;                         
    Uint32                                   CAP4;                         
    Uint16                                   rsvd1[6];                     
    union   ECCTL0_REG                       ECCTL0;                       
    union   ECCTL1_REG                       ECCTL1;                       
    union   ECCTL2_REG                       ECCTL2;                       
    union   ECEINT_REG                       ECEINT;                       
    union   ECFLG_REG                        ECFLG;                        
    union   ECCLR_REG                        ECCLR;                        
    union   ECFRC_REG                        ECFRC;                        
    Uint16                                   rsvd2[4];                     
    union   ECAPSYNCINSEL_REG                ECAPSYNCINSEL;                
};

struct HRCTL_BITS {                     
    Uint16 HRE:1;                       
    Uint16 HRCLKE:1;                    
    Uint16 PRDSEL:1;                    
    Uint16 CALIBSTART:1;                
    Uint16 CALIBSTS:1;                  
    Uint16 CALIBCONT:1;                 
    Uint16 rsvd1:10;                    
    Uint16 rsvd2:16;                    
};

union HRCTL_REG {
    Uint32  all;
    struct  HRCTL_BITS  bit;
};

struct HRINTEN_BITS {                   
    Uint16 rsvd1:1;                     
    Uint16 CALIBDONE:1;                 
    Uint16 CALPRDCHKSTS:1;              
    Uint16 rsvd2:13;                    
    Uint16 rsvd3:16;                    
};

union HRINTEN_REG {
    Uint32  all;
    struct  HRINTEN_BITS  bit;
};

struct HRFLG_BITS {                     
    Uint16 CALIBINT:1;                  
    Uint16 CALIBDONE:1;                 
    Uint16 CALPRDCHKSTS:1;              
    Uint16 rsvd1:13;                    
    Uint16 rsvd2:16;                    
};

union HRFLG_REG {
    Uint32  all;
    struct  HRFLG_BITS  bit;
};

struct HRCLR_BITS {                     
    Uint16 CALIBINT:1;                  
    Uint16 CALIBDONE:1;                 
    Uint16 CALPRDCHKSTS:1;              
    Uint16 rsvd1:13;                    
    Uint16 rsvd2:16;                    
};

union HRCLR_REG {
    Uint32  all;
    struct  HRCLR_BITS  bit;
};

struct HRFRC_BITS {                     
    Uint16 rsvd1:1;                     
    Uint16 CALIBDONE:1;                 
    Uint16 CALPRDCHKSTS:1;              
    Uint16 rsvd2:13;                    
    Uint16 rsvd3:16;                    
};

union HRFRC_REG {
    Uint32  all;
    struct  HRFRC_BITS  bit;
};

struct HRCAP_REGS {
    union   HRCTL_REG                        HRCTL;                        
    Uint16                                   rsvd1[2];                     
    union   HRINTEN_REG                      HRINTEN;                      
    union   HRFLG_REG                        HRFLG;                        
    union   HRCLR_REG                        HRCLR;                        
    union   HRFRC_REG                        HRFRC;                        
    Uint32                                   HRCALPRD;                     
    Uint32                                   HRSYSCLKCTR;                  
    Uint32                                   HRSYSCLKCAP;                  
    Uint32                                   HRCLKCTR;                     
    Uint32                                   HRCLKCAP;                     
};




extern volatile struct ECAP_REGS ECap1Regs;
extern volatile struct ECAP_REGS ECap2Regs;
extern volatile struct ECAP_REGS ECap3Regs;
extern volatile struct HRCAP_REGS HRCap3Regs;



















































struct GCTL0_BITS {                     
    Uint16 EN:1;                        
    Uint16 rsvd1:7;                     
    Uint16 EPGOUT0SEL:1;                
    Uint16 EPGOUT1SEL:1;                
    Uint16 EPGOUT2SEL:1;                
    Uint16 EPGOUT3SEL:1;                
    Uint16 EPGOUT4SEL:1;                
    Uint16 EPGOUT5SEL:1;                
    Uint16 EPGOUT6SEL:1;                
    Uint16 EPGOUT7SEL:1;                
    Uint16 rsvd2:16;                    
};

union GCTL0_REG {
    Uint32  all;
    struct  GCTL0_BITS  bit;
};

struct GCTL1_BITS {                     
    Uint16 SIGGEN0_CLKSEL:3;            
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:3;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:8;                     
    Uint16 rsvd5:16;                    
};

union GCTL1_REG {
    Uint32  all;
    struct  GCTL1_BITS  bit;
};

struct GCTL2_BITS {                     
    Uint16 EPGOUT0_CLKOUTSEL:3;         
    Uint16 rsvd1:1;                     
    Uint16 EPGOUT1_CLKOUTSEL:3;         
    Uint16 rsvd2:1;                     
    Uint16 EPGOUT2_CLKOUTSEL:3;         
    Uint16 rsvd3:1;                     
    Uint16 EPGOUT3_CLKOUTSEL:3;         
    Uint16 rsvd4:1;                     
    Uint16 EPGOUT4_CLKOUTSEL:3;         
    Uint16 rsvd5:1;                     
    Uint16 EPGOUT5_CLKOUTSEL:3;         
    Uint16 rsvd6:1;                     
    Uint16 EPGOUT6_CLKOUTSEL:3;         
    Uint16 rsvd7:1;                     
    Uint16 EPGOUT7_CLKOUTSEL:3;         
    Uint16 rsvd8:1;                     
};

union GCTL2_REG {
    Uint32  all;
    struct  GCTL2_BITS  bit;
};

struct GCTL3_BITS {                     
    Uint16 EPGOUT0_SIGOUTSEL:4;         
    Uint16 EPGOUT1_SIGOUTSEL:4;         
    Uint16 EPGOUT2_SIGOUTSEL:4;         
    Uint16 EPGOUT3_SIGOUTSEL:4;         
    Uint16 EPGOUT4_SIGOUTSEL:4;         
    Uint16 EPGOUT5_SIGOUTSEL:4;         
    Uint16 EPGOUT6_SIGOUTSEL:4;         
    Uint16 EPGOUT7_SIGOUTSEL:4;         
};

union GCTL3_REG {
    Uint32  all;
    struct  GCTL3_BITS  bit;
};

struct EPGLOCK_BITS {                   
    Uint16 GCTL0:1;                     
    Uint16 GCTL1:1;                     
    Uint16 GCTL2:1;                     
    Uint16 GCTL3:1;                     
    Uint16 CLKDIV0_CTL0:1;              
    Uint16 CLKDIV1_CTL0:1;              
    Uint16 SIGGEN0_CTL0:1;              
    Uint16 SIGGEN0_CTL1:1;              
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:6;                     
    Uint16 rsvd4:16;                    
};

union EPGLOCK_REG {
    Uint32  all;
    struct  EPGLOCK_BITS  bit;
};

struct EPGCOMMIT_BITS {                 
    Uint16 GCTL0:1;                     
    Uint16 GCTL1:1;                     
    Uint16 GCTL2:1;                     
    Uint16 GCTL3:1;                     
    Uint16 CLKDIV0_CTL0:1;              
    Uint16 CLKDIV1_CTL0:1;              
    Uint16 SIGGEN0_CTL0:1;              
    Uint16 SIGGEN0_CTL1:1;              
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:6;                     
    Uint16 rsvd4:16;                    
};

union EPGCOMMIT_REG {
    Uint32  all;
    struct  EPGCOMMIT_BITS  bit;
};

struct GINTSTS_BITS {                   
    Uint16 INT:1;                       
    Uint16 SIGGEN0_DONE:1;              
    Uint16 SIGGEN0_FILL:1;              
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:11;                    
    Uint16 rsvd4:16;                    
};

union GINTSTS_REG {
    Uint32  all;
    struct  GINTSTS_BITS  bit;
};

struct GINTEN_BITS {                    
    Uint16 rsvd1:1;                     
    Uint16 SIGGEN0_DONE:1;              
    Uint16 SIGGEN0_FILL:1;              
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:11;                    
    Uint16 rsvd5:16;                    
};

union GINTEN_REG {
    Uint32  all;
    struct  GINTEN_BITS  bit;
};

struct GINTCLR_BITS {                   
    Uint16 INT:1;                       
    Uint16 SIGGEN0_DONE:1;              
    Uint16 SIGGEN0_FILL:1;              
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:11;                    
    Uint16 rsvd4:16;                    
};

union GINTCLR_REG {
    Uint32  all;
    struct  GINTCLR_BITS  bit;
};

struct GINTFRC_BITS {                   
    Uint16 rsvd1:1;                     
    Uint16 SIGGEN0_DONE:1;              
    Uint16 SIGGEN0_FILL:1;              
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:11;                    
    Uint16 rsvd5:16;                    
};

union GINTFRC_REG {
    Uint32  all;
    struct  GINTFRC_BITS  bit;
};

struct CLKDIV0_CTL0_BITS {              
    Uint16 PRD:8;                       
    Uint16 rsvd1:8;                     
    Uint16 CLKSTOP:3;                   
    Uint16 rsvd2:13;                    
};

union CLKDIV0_CTL0_REG {
    Uint32  all;
    struct  CLKDIV0_CTL0_BITS  bit;
};

struct CLKDIV0_CLKOFFSET_BITS {         
    Uint16 CLK0OFFSET:8;                
    Uint16 CLK1OFFSET:8;                
    Uint16 CLK2OFFSET:8;                
    Uint16 CLK3OFFSET:8;                
};

union CLKDIV0_CLKOFFSET_REG {
    Uint32  all;
    struct  CLKDIV0_CLKOFFSET_BITS  bit;
};

struct CLKDIV1_CTL0_BITS {              
    Uint16 PRD:8;                       
    Uint16 rsvd1:8;                     
    Uint16 CLKSTOP:3;                   
    Uint16 rsvd2:13;                    
};

union CLKDIV1_CTL0_REG {
    Uint32  all;
    struct  CLKDIV1_CTL0_BITS  bit;
};

struct CLKDIV1_CLKOFFSET_BITS {         
    Uint16 CLK0OFFSET:8;                
    Uint16 CLK1OFFSET:8;                
    Uint16 CLK2OFFSET:8;                
    Uint16 CLK3OFFSET:8;                
};

union CLKDIV1_CLKOFFSET_REG {
    Uint32  all;
    struct  CLKDIV1_CLKOFFSET_BITS  bit;
};

struct SIGGEN0_CTL0_BITS {              
    Uint16 MODE:4;                      
    Uint16 EN:1;                        
    Uint16 BRIN:1;                      
    Uint16 BROUT:1;                     
    Uint16 rsvd1:9;                     
    Uint16 BITLENGTH:8;                 
    Uint16 rsvd2:8;                     
};

union SIGGEN0_CTL0_REG {
    Uint32  all;
    struct  SIGGEN0_CTL0_BITS  bit;
};

struct SIGGEN0_CTL1_BITS {              
    Uint16 DATA0_INSEL:4;               
    Uint16 rsvd1:12;                    
    Uint16 rsvd2:12;                    
    Uint16 DATA63_INSEL:4;              
};

union SIGGEN0_CTL1_REG {
    Uint32  all;
    struct  SIGGEN0_CTL1_BITS  bit;
};

struct EPG_REGS {
    union   GCTL0_REG                        GCTL0;                        
    union   GCTL1_REG                        GCTL1;                        
    union   GCTL2_REG                        GCTL2;                        
    union   GCTL3_REG                        GCTL3;                        
    union   EPGLOCK_REG                      EPGLOCK;                      
    union   EPGCOMMIT_REG                    EPGCOMMIT;                    
    union   GINTSTS_REG                      GINTSTS;                      
    union   GINTEN_REG                       GINTEN;                       
    union   GINTCLR_REG                      GINTCLR;                      
    union   GINTFRC_REG                      GINTFRC;                      
    Uint16                                   rsvd1[4];                     
    union   CLKDIV0_CTL0_REG                 CLKDIV0_CTL0;                 
    Uint16                                   rsvd2[4];                     
    union   CLKDIV0_CLKOFFSET_REG            CLKDIV0_CLKOFFSET;            
    Uint16                                   rsvd3[4];                     
    union   CLKDIV1_CTL0_REG                 CLKDIV1_CTL0;                 
    Uint16                                   rsvd4[4];                     
    union   CLKDIV1_CLKOFFSET_REG            CLKDIV1_CLKOFFSET;            
    Uint16                                   rsvd5[4];                     
    union   SIGGEN0_CTL0_REG                 SIGGEN0_CTL0;                 
    union   SIGGEN0_CTL1_REG                 SIGGEN0_CTL1;                 
    Uint16                                   rsvd6[4];                     
    Uint32                                   SIGGEN0_DATA0;                
    Uint32                                   SIGGEN0_DATA1;                
    Uint32                                   SIGGEN0_DATA0_ACTIVE;         
    Uint32                                   SIGGEN0_DATA1_ACTIVE;         
};

struct EPGMXSEL0_BITS {                 
    Uint16 SEL0:1;                      
    Uint16 SEL1:1;                      
    Uint16 SEL2:1;                      
    Uint16 SEL3:1;                      
    Uint16 SEL4:1;                      
    Uint16 SEL5:1;                      
    Uint16 SEL6:1;                      
    Uint16 SEL7:1;                      
    Uint16 SEL8:1;                      
    Uint16 SEL9:1;                      
    Uint16 SEL10:1;                     
    Uint16 SEL11:1;                     
    Uint16 SEL12:1;                     
    Uint16 SEL13:1;                     
    Uint16 SEL14:1;                     
    Uint16 SEL15:1;                     
    Uint16 SEL16:1;                     
    Uint16 SEL17:1;                     
    Uint16 SEL18:1;                     
    Uint16 SEL19:1;                     
    Uint16 SEL20:1;                     
    Uint16 SEL21:1;                     
    Uint16 SEL22:1;                     
    Uint16 SEL23:1;                     
    Uint16 SEL24:1;                     
    Uint16 SEL25:1;                     
    Uint16 SEL26:1;                     
    Uint16 SEL27:1;                     
    Uint16 SEL28:1;                     
    Uint16 SEL29:1;                     
    Uint16 SEL30:1;                     
    Uint16 SEL31:1;                     
};

union EPGMXSEL0_REG {
    Uint32  all;
    struct  EPGMXSEL0_BITS  bit;
};

struct EPGMXSELLOCK_BITS {              
    Uint16 EPGMXSEL0:1;                 
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:14;                    
    Uint16 rsvd3:16;                    
};

union EPGMXSELLOCK_REG {
    Uint32  all;
    struct  EPGMXSELLOCK_BITS  bit;
};

struct EPGMXSELCOMMIT_BITS {            
    Uint16 EPGMXSEL0:1;                 
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:14;                    
    Uint16 rsvd3:16;                    
};

union EPGMXSELCOMMIT_REG {
    Uint32  all;
    struct  EPGMXSELCOMMIT_BITS  bit;
};

struct EPG_MUX_REGS {
    union   EPGMXSEL0_REG                    EPGMXSEL0;                    
    Uint16                                   rsvd1[10];                    
    union   EPGMXSELLOCK_REG                 EPGMXSELLOCK;                 
    union   EPGMXSELCOMMIT_REG               EPGMXSELCOMMIT;               
};




extern volatile struct EPG_REGS Epg1Regs;
extern volatile struct EPG_MUX_REGS Epg1MuxRegs;



















































struct TBCTL_BITS {                     
    Uint16 CTRMODE:2;                   
    Uint16 PHSEN:1;                     
    Uint16 PRDLD:1;                     
    Uint16 rsvd1:2;                     
    Uint16 SWFSYNC:1;                   
    Uint16 HSPCLKDIV:3;                 
    Uint16 CLKDIV:3;                    
    Uint16 PHSDIR:1;                    
    Uint16 FREE_SOFT:2;                 
};

union TBCTL_REG {
    Uint16  all;
    struct  TBCTL_BITS  bit;
};

struct TBCTL2_BITS {                    
    Uint16 rsvd1:5;                     
    Uint16 rsvd2:1;                     
    Uint16 OSHTSYNCMODE:1;              
    Uint16 OSHTSYNC:1;                  
    Uint16 rsvd3:4;                     
    Uint16 rsvd4:2;                     
    Uint16 PRDLDSYNC:2;                 
};

union TBCTL2_REG {
    Uint16  all;
    struct  TBCTL2_BITS  bit;
};

struct EPWMSYNCINSEL_BITS {             
    Uint16 SEL:5;                       
    Uint16 rsvd1:11;                    
};

union EPWMSYNCINSEL_REG {
    Uint16  all;
    struct  EPWMSYNCINSEL_BITS  bit;
};

struct TBSTS_BITS {                     
    Uint16 CTRDIR:1;                    
    Uint16 SYNCI:1;                     
    Uint16 CTRMAX:1;                    
    Uint16 rsvd1:13;                    
};

union TBSTS_REG {
    Uint16  all;
    struct  TBSTS_BITS  bit;
};

struct EPWMSYNCOUTEN_BITS {             
    Uint16 SWEN:1;                      
    Uint16 ZEROEN:1;                    
    Uint16 CMPBEN:1;                    
    Uint16 CMPCEN:1;                    
    Uint16 CMPDEN:1;                    
    Uint16 DCAEVT1EN:1;                 
    Uint16 DCBEVT1EN:1;                 
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:8;                     
};

union EPWMSYNCOUTEN_REG {
    Uint16  all;
    struct  EPWMSYNCOUTEN_BITS  bit;
};

struct TBCTL3_BITS {                    
    Uint16 OSSFRCEN:1;                  
    Uint16 rsvd1:15;                    
};

union TBCTL3_REG {
    Uint16  all;
    struct  TBCTL3_BITS  bit;
};

struct CMPCTL_BITS {                    
    Uint16 LOADAMODE:2;                 
    Uint16 LOADBMODE:2;                 
    Uint16 SHDWAMODE:1;                 
    Uint16 rsvd1:1;                     
    Uint16 SHDWBMODE:1;                 
    Uint16 rsvd2:1;                     
    Uint16 SHDWAFULL:1;                 
    Uint16 SHDWBFULL:1;                 
    Uint16 LOADASYNC:2;                 
    Uint16 LOADBSYNC:2;                 
    Uint16 rsvd3:2;                     
};

union CMPCTL_REG {
    Uint16  all;
    struct  CMPCTL_BITS  bit;
};

struct CMPCTL2_BITS {                   
    Uint16 LOADCMODE:2;                 
    Uint16 LOADDMODE:2;                 
    Uint16 SHDWCMODE:1;                 
    Uint16 rsvd1:1;                     
    Uint16 SHDWDMODE:1;                 
    Uint16 rsvd2:3;                     
    Uint16 LOADCSYNC:2;                 
    Uint16 LOADDSYNC:2;                 
    Uint16 rsvd3:2;                     
};

union CMPCTL2_REG {
    Uint16  all;
    struct  CMPCTL2_BITS  bit;
};

struct DBCTL_BITS {                     
    Uint16 OUT_MODE:2;                  
    Uint16 POLSEL:2;                    
    Uint16 IN_MODE:2;                   
    Uint16 LOADREDMODE:2;               
    Uint16 LOADFEDMODE:2;               
    Uint16 SHDWDBREDMODE:1;             
    Uint16 SHDWDBFEDMODE:1;             
    Uint16 OUTSWAP:2;                   
    Uint16 DEDB_MODE:1;                 
    Uint16 HALFCYCLE:1;                 
};

union DBCTL_REG {
    Uint16  all;
    struct  DBCTL_BITS  bit;
};

struct DBCTL2_BITS {                    
    Uint16 LOADDBCTLMODE:2;             
    Uint16 SHDWDBCTLMODE:1;             
    Uint16 rsvd1:13;                    
};

union DBCTL2_REG {
    Uint16  all;
    struct  DBCTL2_BITS  bit;
};

struct AQCTL_BITS {                     
    Uint16 LDAQAMODE:2;                 
    Uint16 LDAQBMODE:2;                 
    Uint16 SHDWAQAMODE:1;               
    Uint16 rsvd1:1;                     
    Uint16 SHDWAQBMODE:1;               
    Uint16 rsvd2:1;                     
    Uint16 LDAQASYNC:2;                 
    Uint16 LDAQBSYNC:2;                 
    Uint16 rsvd3:4;                     
};

union AQCTL_REG {
    Uint16  all;
    struct  AQCTL_BITS  bit;
};

struct AQTSRCSEL_BITS {                 
    Uint16 T1SEL:4;                     
    Uint16 T2SEL:4;                     
    Uint16 rsvd1:8;                     
};

union AQTSRCSEL_REG {
    Uint16  all;
    struct  AQTSRCSEL_BITS  bit;
};

struct PCCTL_BITS {                     
    Uint16 CHPEN:1;                     
    Uint16 OSHTWTH:4;                   
    Uint16 CHPFREQ:3;                   
    Uint16 CHPDUTY:3;                   
    Uint16 rsvd1:5;                     
};

union PCCTL_REG {
    Uint16  all;
    struct  PCCTL_BITS  bit;
};

struct VCAPCTL_BITS {                   
    Uint16 VCAPE:1;                     
    Uint16 VCAPSTART:1;                 
    Uint16 TRIGSEL:3;                   
    Uint16 rsvd1:2;                     
    Uint16 VDELAYDIV:3;                 
    Uint16 EDGEFILTDLYSEL:1;            
    Uint16 rsvd2:5;                     
};

union VCAPCTL_REG {
    Uint16  all;
    struct  VCAPCTL_BITS  bit;
};

struct VCNTCFG_BITS {                   
    Uint16 STARTEDGE:4;                 
    Uint16 rsvd1:3;                     
    Uint16 STARTEDGESTS:1;              
    Uint16 STOPEDGE:4;                  
    Uint16 rsvd2:3;                     
    Uint16 STOPEDGESTS:1;               
};

union VCNTCFG_REG {
    Uint16  all;
    struct  VCNTCFG_BITS  bit;
};

struct HRCNFG_BITS {                    
    Uint16 EDGMODE:2;                   
    Uint16 CTLMODE:1;                   
    Uint16 HRLOAD:2;                    
    Uint16 SELOUTB:1;                   
    Uint16 AUTOCONV:1;                  
    Uint16 SWAPAB:1;                    
    Uint16 EDGMODEB:2;                  
    Uint16 CTLMODEB:1;                  
    Uint16 HRLOADB:2;                   
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:2;                     
};

union HRCNFG_REG {
    Uint16  all;
    struct  HRCNFG_BITS  bit;
};

struct HRPWR_BITS {                     
    Uint16 rsvd1:2;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:4;                     
    Uint16 rsvd7:5;                     
    Uint16 CALPWRON:1;                  
};

union HRPWR_REG {
    Uint16  all;
    struct  HRPWR_BITS  bit;
};

struct HRMSTEP_BITS {                   
    Uint16 HRMSTEP:8;                   
    Uint16 rsvd1:8;                     
};

union HRMSTEP_REG {
    Uint16  all;
    struct  HRMSTEP_BITS  bit;
};

struct HRCNFG2_BITS {                   
    Uint16 EDGMODEDB:2;                 
    Uint16 CTLMODEDBRED:2;              
    Uint16 CTLMODEDBFED:2;              
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
};

union HRCNFG2_REG {
    Uint16  all;
    struct  HRCNFG2_BITS  bit;
};

struct HRPCTL_BITS {                    
    Uint16 HRPE:1;                      
    Uint16 PWMSYNCSEL:1;                
    Uint16 TBPHSHRLOADE:1;              
    Uint16 rsvd1:1;                     
    Uint16 PWMSYNCSELX:3;               
    Uint16 rsvd2:9;                     
};

union HRPCTL_REG {
    Uint16  all;
    struct  HRPCTL_BITS  bit;
};

struct TRREM_BITS {                     
    Uint16 TRREM:11;                    
    Uint16 rsvd1:5;                     
};

union TRREM_REG {
    Uint16  all;
    struct  TRREM_BITS  bit;
};

struct GLDCTL_BITS {                    
    Uint16 GLD:1;                       
    Uint16 GLDMODE:4;                   
    Uint16 OSHTMODE:1;                  
    Uint16 rsvd1:1;                     
    Uint16 GLDPRD:3;                    
    Uint16 GLDCNT:3;                    
    Uint16 rsvd2:3;                     
};

union GLDCTL_REG {
    Uint16  all;
    struct  GLDCTL_BITS  bit;
};

struct GLDCFG_BITS {                    
    Uint16 TBPRD_TBPRDHR:1;             
    Uint16 CMPA_CMPAHR:1;               
    Uint16 CMPB_CMPBHR:1;               
    Uint16 CMPC:1;                      
    Uint16 CMPD:1;                      
    Uint16 DBRED_DBREDHR:1;             
    Uint16 DBFED_DBFEDHR:1;             
    Uint16 DBCTL:1;                     
    Uint16 AQCTLA_AQCTLA2:1;            
    Uint16 AQCTLB_AQCTLB2:1;            
    Uint16 AQCSFRC:1;                   
    Uint16 rsvd1:5;                     
};

union GLDCFG_REG {
    Uint16  all;
    struct  GLDCFG_BITS  bit;
};

struct EPWMXLINK_BITS {                 
    Uint16 TBPRDLINK:4;                 
    Uint16 CMPALINK:4;                  
    Uint16 CMPBLINK:4;                  
    Uint16 CMPCLINK:4;                  
    Uint16 CMPDLINK:4;                  
    Uint16 rsvd1:8;                     
    Uint16 GLDCTL2LINK:4;               
};

union EPWMXLINK_REG {
    Uint32  all;
    struct  EPWMXLINK_BITS  bit;
};

struct AQCTLA_BITS {                    
    Uint16 ZRO:2;                       
    Uint16 PRD:2;                       
    Uint16 CAU:2;                       
    Uint16 CAD:2;                       
    Uint16 CBU:2;                       
    Uint16 CBD:2;                       
    Uint16 rsvd1:4;                     
};

union AQCTLA_REG {
    Uint16  all;
    struct  AQCTLA_BITS  bit;
};

struct AQCTLA2_BITS {                   
    Uint16 T1U:2;                       
    Uint16 T1D:2;                       
    Uint16 T2U:2;                       
    Uint16 T2D:2;                       
    Uint16 rsvd1:8;                     
};

union AQCTLA2_REG {
    Uint16  all;
    struct  AQCTLA2_BITS  bit;
};

struct AQCTLB_BITS {                    
    Uint16 ZRO:2;                       
    Uint16 PRD:2;                       
    Uint16 CAU:2;                       
    Uint16 CAD:2;                       
    Uint16 CBU:2;                       
    Uint16 CBD:2;                       
    Uint16 rsvd1:4;                     
};

union AQCTLB_REG {
    Uint16  all;
    struct  AQCTLB_BITS  bit;
};

struct AQCTLB2_BITS {                   
    Uint16 T1U:2;                       
    Uint16 T1D:2;                       
    Uint16 T2U:2;                       
    Uint16 T2D:2;                       
    Uint16 rsvd1:8;                     
};

union AQCTLB2_REG {
    Uint16  all;
    struct  AQCTLB2_BITS  bit;
};

struct AQSFRC_BITS {                    
    Uint16 ACTSFA:2;                    
    Uint16 OTSFA:1;                     
    Uint16 ACTSFB:2;                    
    Uint16 OTSFB:1;                     
    Uint16 RLDCSF:2;                    
    Uint16 rsvd1:8;                     
};

union AQSFRC_REG {
    Uint16  all;
    struct  AQSFRC_BITS  bit;
};

struct AQCSFRC_BITS {                   
    Uint16 CSFA:2;                      
    Uint16 CSFB:2;                      
    Uint16 rsvd1:12;                    
};

union AQCSFRC_REG {
    Uint16  all;
    struct  AQCSFRC_BITS  bit;
};

struct DBREDHR_BITS {                   
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:7;                     
    Uint16 rsvd3:1;                     
    Uint16 DBREDHR:7;                   
};

union DBREDHR_REG {
    Uint16  all;
    struct  DBREDHR_BITS  bit;
};

struct DBRED_BITS {                     
    Uint16 DBRED:14;                    
    Uint16 rsvd1:2;                     
};

union DBRED_REG {
    Uint16  all;
    struct  DBRED_BITS  bit;
};

struct DBFEDHR_BITS {                   
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:7;                     
    Uint16 rsvd3:1;                     
    Uint16 DBFEDHR:7;                   
};

union DBFEDHR_REG {
    Uint16  all;
    struct  DBFEDHR_BITS  bit;
};

struct DBFED_BITS {                     
    Uint16 DBFED:14;                    
    Uint16 rsvd1:2;                     
};

union DBFED_REG {
    Uint16  all;
    struct  DBFED_BITS  bit;
};

struct TBPHS_BITS {                     
    Uint16 TBPHSHR:16;                  
    Uint16 TBPHS:16;                    
};

union TBPHS_REG {
    Uint32  all;
    struct  TBPHS_BITS  bit;
};

struct CMPA_BITS {                      
    Uint16 CMPAHR:16;                   
    Uint16 CMPA:16;                     
};

union CMPA_REG {
    Uint32  all;
    struct  CMPA_BITS  bit;
};

struct CMPB_BITS {                      
    Uint16 CMPBHR:16;                   
    Uint16 CMPB:16;                     
};

union CMPB_REG {
    Uint32  all;
    struct  CMPB_BITS  bit;
};

struct GLDCTL2_BITS {                   
    Uint16 OSHTLD:1;                    
    Uint16 GFRCLD:1;                    
    Uint16 rsvd1:14;                    
};

union GLDCTL2_REG {
    Uint16  all;
    struct  GLDCTL2_BITS  bit;
};

struct TZSEL_BITS {                     
    Uint16 CBC1:1;                      
    Uint16 CBC2:1;                      
    Uint16 CBC3:1;                      
    Uint16 CBC4:1;                      
    Uint16 CBC5:1;                      
    Uint16 CBC6:1;                      
    Uint16 DCAEVT2:1;                   
    Uint16 DCBEVT2:1;                   
    Uint16 OSHT1:1;                     
    Uint16 OSHT2:1;                     
    Uint16 OSHT3:1;                     
    Uint16 OSHT4:1;                     
    Uint16 OSHT5:1;                     
    Uint16 OSHT6:1;                     
    Uint16 DCAEVT1:1;                   
    Uint16 DCBEVT1:1;                   
};

union TZSEL_REG {
    Uint16  all;
    struct  TZSEL_BITS  bit;
};

struct TZDCSEL_BITS {                   
    Uint16 DCAEVT1:3;                   
    Uint16 DCAEVT2:3;                   
    Uint16 DCBEVT1:3;                   
    Uint16 DCBEVT2:3;                   
    Uint16 rsvd1:4;                     
};

union TZDCSEL_REG {
    Uint16  all;
    struct  TZDCSEL_BITS  bit;
};

struct TZCTL_BITS {                     
    Uint16 TZA:2;                       
    Uint16 TZB:2;                       
    Uint16 DCAEVT1:2;                   
    Uint16 DCAEVT2:2;                   
    Uint16 DCBEVT1:2;                   
    Uint16 DCBEVT2:2;                   
    Uint16 rsvd1:4;                     
};

union TZCTL_REG {
    Uint16  all;
    struct  TZCTL_BITS  bit;
};

struct TZCTL2_BITS {                    
    Uint16 TZAU:3;                      
    Uint16 TZAD:3;                      
    Uint16 TZBU:3;                      
    Uint16 TZBD:3;                      
    Uint16 rsvd1:3;                     
    Uint16 ETZE:1;                      
};

union TZCTL2_REG {
    Uint16  all;
    struct  TZCTL2_BITS  bit;
};

struct TZCTLDCA_BITS {                  
    Uint16 DCAEVT1U:3;                  
    Uint16 DCAEVT1D:3;                  
    Uint16 DCAEVT2U:3;                  
    Uint16 DCAEVT2D:3;                  
    Uint16 rsvd1:4;                     
};

union TZCTLDCA_REG {
    Uint16  all;
    struct  TZCTLDCA_BITS  bit;
};

struct TZCTLDCB_BITS {                  
    Uint16 DCBEVT1U:3;                  
    Uint16 DCBEVT1D:3;                  
    Uint16 DCBEVT2U:3;                  
    Uint16 DCBEVT2D:3;                  
    Uint16 rsvd1:4;                     
};

union TZCTLDCB_REG {
    Uint16  all;
    struct  TZCTLDCB_BITS  bit;
};

struct TZEINT_BITS {                    
    Uint16 rsvd1:1;                     
    Uint16 CBC:1;                       
    Uint16 OST:1;                       
    Uint16 DCAEVT1:1;                   
    Uint16 DCAEVT2:1;                   
    Uint16 DCBEVT1:1;                   
    Uint16 DCBEVT2:1;                   
    Uint16 rsvd2:9;                     
};

union TZEINT_REG {
    Uint16  all;
    struct  TZEINT_BITS  bit;
};

struct TZFLG_BITS {                     
    Uint16 INT:1;                       
    Uint16 CBC:1;                       
    Uint16 OST:1;                       
    Uint16 DCAEVT1:1;                   
    Uint16 DCAEVT2:1;                   
    Uint16 DCBEVT1:1;                   
    Uint16 DCBEVT2:1;                   
    Uint16 rsvd1:9;                     
};

union TZFLG_REG {
    Uint16  all;
    struct  TZFLG_BITS  bit;
};

struct TZCBCFLG_BITS {                  
    Uint16 CBC1:1;                      
    Uint16 CBC2:1;                      
    Uint16 CBC3:1;                      
    Uint16 CBC4:1;                      
    Uint16 CBC5:1;                      
    Uint16 CBC6:1;                      
    Uint16 DCAEVT2:1;                   
    Uint16 DCBEVT2:1;                   
    Uint16 rsvd1:8;                     
};

union TZCBCFLG_REG {
    Uint16  all;
    struct  TZCBCFLG_BITS  bit;
};

struct TZOSTFLG_BITS {                  
    Uint16 OST1:1;                      
    Uint16 OST2:1;                      
    Uint16 OST3:1;                      
    Uint16 OST4:1;                      
    Uint16 OST5:1;                      
    Uint16 OST6:1;                      
    Uint16 DCAEVT1:1;                   
    Uint16 DCBEVT1:1;                   
    Uint16 rsvd1:8;                     
};

union TZOSTFLG_REG {
    Uint16  all;
    struct  TZOSTFLG_BITS  bit;
};

struct TZCLR_BITS {                     
    Uint16 INT:1;                       
    Uint16 CBC:1;                       
    Uint16 OST:1;                       
    Uint16 DCAEVT1:1;                   
    Uint16 DCAEVT2:1;                   
    Uint16 DCBEVT1:1;                   
    Uint16 DCBEVT2:1;                   
    Uint16 rsvd1:7;                     
    Uint16 CBCPULSE:2;                  
};

union TZCLR_REG {
    Uint16  all;
    struct  TZCLR_BITS  bit;
};

struct TZCBCCLR_BITS {                  
    Uint16 CBC1:1;                      
    Uint16 CBC2:1;                      
    Uint16 CBC3:1;                      
    Uint16 CBC4:1;                      
    Uint16 CBC5:1;                      
    Uint16 CBC6:1;                      
    Uint16 DCAEVT2:1;                   
    Uint16 DCBEVT2:1;                   
    Uint16 rsvd1:8;                     
};

union TZCBCCLR_REG {
    Uint16  all;
    struct  TZCBCCLR_BITS  bit;
};

struct TZOSTCLR_BITS {                  
    Uint16 OST1:1;                      
    Uint16 OST2:1;                      
    Uint16 OST3:1;                      
    Uint16 OST4:1;                      
    Uint16 OST5:1;                      
    Uint16 OST6:1;                      
    Uint16 DCAEVT1:1;                   
    Uint16 DCBEVT1:1;                   
    Uint16 rsvd1:8;                     
};

union TZOSTCLR_REG {
    Uint16  all;
    struct  TZOSTCLR_BITS  bit;
};

struct TZFRC_BITS {                     
    Uint16 rsvd1:1;                     
    Uint16 CBC:1;                       
    Uint16 OST:1;                       
    Uint16 DCAEVT1:1;                   
    Uint16 DCAEVT2:1;                   
    Uint16 DCBEVT1:1;                   
    Uint16 DCBEVT2:1;                   
    Uint16 rsvd2:9;                     
};

union TZFRC_REG {
    Uint16  all;
    struct  TZFRC_BITS  bit;
};

struct ETSEL_BITS {                     
    Uint16 INTSEL:3;                    
    Uint16 INTEN:1;                     
    Uint16 SOCASELCMP:1;                
    Uint16 SOCBSELCMP:1;                
    Uint16 INTSELCMP:1;                 
    Uint16 rsvd1:1;                     
    Uint16 SOCASEL:3;                   
    Uint16 SOCAEN:1;                    
    Uint16 SOCBSEL:3;                   
    Uint16 SOCBEN:1;                    
};

union ETSEL_REG {
    Uint16  all;
    struct  ETSEL_BITS  bit;
};

struct ETPS_BITS {                      
    Uint16 INTPRD:2;                    
    Uint16 INTCNT:2;                    
    Uint16 INTPSSEL:1;                  
    Uint16 SOCPSSEL:1;                  
    Uint16 rsvd1:2;                     
    Uint16 SOCAPRD:2;                   
    Uint16 SOCACNT:2;                   
    Uint16 SOCBPRD:2;                   
    Uint16 SOCBCNT:2;                   
};

union ETPS_REG {
    Uint16  all;
    struct  ETPS_BITS  bit;
};

struct ETFLG_BITS {                     
    Uint16 INT:1;                       
    Uint16 rsvd1:1;                     
    Uint16 SOCA:1;                      
    Uint16 SOCB:1;                      
    Uint16 rsvd2:12;                    
};

union ETFLG_REG {
    Uint16  all;
    struct  ETFLG_BITS  bit;
};

struct ETCLR_BITS {                     
    Uint16 INT:1;                       
    Uint16 rsvd1:1;                     
    Uint16 SOCA:1;                      
    Uint16 SOCB:1;                      
    Uint16 rsvd2:12;                    
};

union ETCLR_REG {
    Uint16  all;
    struct  ETCLR_BITS  bit;
};

struct ETFRC_BITS {                     
    Uint16 INT:1;                       
    Uint16 rsvd1:1;                     
    Uint16 SOCA:1;                      
    Uint16 SOCB:1;                      
    Uint16 rsvd2:12;                    
};

union ETFRC_REG {
    Uint16  all;
    struct  ETFRC_BITS  bit;
};

struct ETINTPS_BITS {                   
    Uint16 INTPRD2:4;                   
    Uint16 INTCNT2:4;                   
    Uint16 rsvd1:8;                     
};

union ETINTPS_REG {
    Uint16  all;
    struct  ETINTPS_BITS  bit;
};

struct ETSOCPS_BITS {                   
    Uint16 SOCAPRD2:4;                  
    Uint16 SOCACNT2:4;                  
    Uint16 SOCBPRD2:4;                  
    Uint16 SOCBCNT2:4;                  
};

union ETSOCPS_REG {
    Uint16  all;
    struct  ETSOCPS_BITS  bit;
};

struct ETCNTINITCTL_BITS {              
    Uint16 rsvd1:10;                    
    Uint16 INTINITFRC:1;                
    Uint16 SOCAINITFRC:1;               
    Uint16 SOCBINITFRC:1;               
    Uint16 INTINITEN:1;                 
    Uint16 SOCAINITEN:1;                
    Uint16 SOCBINITEN:1;                
};

union ETCNTINITCTL_REG {
    Uint16  all;
    struct  ETCNTINITCTL_BITS  bit;
};

struct ETCNTINIT_BITS {                 
    Uint16 INTINIT:4;                   
    Uint16 SOCAINIT:4;                  
    Uint16 SOCBINIT:4;                  
    Uint16 rsvd1:4;                     
};

union ETCNTINIT_REG {
    Uint16  all;
    struct  ETCNTINIT_BITS  bit;
};

struct DCTRIPSEL_BITS {                 
    Uint16 DCAHCOMPSEL:4;               
    Uint16 DCALCOMPSEL:4;               
    Uint16 DCBHCOMPSEL:4;               
    Uint16 DCBLCOMPSEL:4;               
};

union DCTRIPSEL_REG {
    Uint16  all;
    struct  DCTRIPSEL_BITS  bit;
};

struct DCACTL_BITS {                    
    Uint16 EVT1SRCSEL:1;                
    Uint16 EVT1FRCSYNCSEL:1;            
    Uint16 EVT1SOCE:1;                  
    Uint16 EVT1SYNCE:1;                 
    Uint16 EVT1LATSEL:1;                
    Uint16 EVT1LATCLRSEL:2;             
    Uint16 EVT1LAT:1;                   
    Uint16 EVT2SRCSEL:1;                
    Uint16 EVT2FRCSYNCSEL:1;            
    Uint16 rsvd1:2;                     
    Uint16 EVT2LATSEL:1;                
    Uint16 EVT2LATCLRSEL:2;             
    Uint16 EVT2LAT:1;                   
};

union DCACTL_REG {
    Uint16  all;
    struct  DCACTL_BITS  bit;
};

struct DCBCTL_BITS {                    
    Uint16 EVT1SRCSEL:1;                
    Uint16 EVT1FRCSYNCSEL:1;            
    Uint16 EVT1SOCE:1;                  
    Uint16 EVT1SYNCE:1;                 
    Uint16 EVT1LATSEL:1;                
    Uint16 EVT1LATCLRSEL:2;             
    Uint16 EVT1LAT:1;                   
    Uint16 EVT2SRCSEL:1;                
    Uint16 EVT2FRCSYNCSEL:1;            
    Uint16 rsvd1:2;                     
    Uint16 EVT2LATSEL:1;                
    Uint16 EVT2LATCLRSEL:2;             
    Uint16 EVT2LAT:1;                   
};

union DCBCTL_REG {
    Uint16  all;
    struct  DCBCTL_BITS  bit;
};

struct DCFCTL_BITS {                    
    Uint16 SRCSEL:2;                    
    Uint16 BLANKE:1;                    
    Uint16 BLANKINV:1;                  
    Uint16 PULSESEL:2;                  
    Uint16 EDGEFILTSEL:1;               
    Uint16 rsvd1:1;                     
    Uint16 EDGEMODE:2;                  
    Uint16 EDGECOUNT:3;                 
    Uint16 EDGESTATUS:3;                
};

union DCFCTL_REG {
    Uint16  all;
    struct  DCFCTL_BITS  bit;
};

struct DCCAPCTL_BITS {                  
    Uint16 CAPE:1;                      
    Uint16 SHDWMODE:1;                  
    Uint16 rsvd1:11;                    
    Uint16 CAPSTS:1;                    
    Uint16 CAPCLR:1;                    
    Uint16 CAPMODE:1;                   
};

union DCCAPCTL_REG {
    Uint16  all;
    struct  DCCAPCTL_BITS  bit;
};

struct BLANKPULSEMIXSEL_BITS {          
    Uint16 ZRO:1;                       
    Uint16 PRD:1;                       
    Uint16 CAU:1;                       
    Uint16 CAD:1;                       
    Uint16 CBU:1;                       
    Uint16 CBD:1;                       
    Uint16 CCU:1;                       
    Uint16 CCD:1;                       
    Uint16 CDU:1;                       
    Uint16 CDD:1;                       
    Uint16 rsvd1:6;                     
};

union BLANKPULSEMIXSEL_REG {
    Uint16  all;
    struct  BLANKPULSEMIXSEL_BITS  bit;
};

struct DCAHTRIPSEL_BITS {               
    Uint16 TRIPINPUT1:1;                
    Uint16 TRIPINPUT2:1;                
    Uint16 TRIPINPUT3:1;                
    Uint16 TRIPINPUT4:1;                
    Uint16 TRIPINPUT5:1;                
    Uint16 TRIPINPUT6:1;                
    Uint16 TRIPINPUT7:1;                
    Uint16 TRIPINPUT8:1;                
    Uint16 TRIPINPUT9:1;                
    Uint16 TRIPINPUT10:1;               
    Uint16 TRIPINPUT11:1;               
    Uint16 TRIPINPUT12:1;               
    Uint16 rsvd1:1;                     
    Uint16 TRIPINPUT14:1;               
    Uint16 TRIPINPUT15:1;               
    Uint16 rsvd2:1;                     
};

union DCAHTRIPSEL_REG {
    Uint16  all;
    struct  DCAHTRIPSEL_BITS  bit;
};

struct DCALTRIPSEL_BITS {               
    Uint16 TRIPINPUT1:1;                
    Uint16 TRIPINPUT2:1;                
    Uint16 TRIPINPUT3:1;                
    Uint16 TRIPINPUT4:1;                
    Uint16 TRIPINPUT5:1;                
    Uint16 TRIPINPUT6:1;                
    Uint16 TRIPINPUT7:1;                
    Uint16 TRIPINPUT8:1;                
    Uint16 TRIPINPUT9:1;                
    Uint16 TRIPINPUT10:1;               
    Uint16 TRIPINPUT11:1;               
    Uint16 TRIPINPUT12:1;               
    Uint16 rsvd1:1;                     
    Uint16 TRIPINPUT14:1;               
    Uint16 TRIPINPUT15:1;               
    Uint16 rsvd2:1;                     
};

union DCALTRIPSEL_REG {
    Uint16  all;
    struct  DCALTRIPSEL_BITS  bit;
};

struct DCBHTRIPSEL_BITS {               
    Uint16 TRIPINPUT1:1;                
    Uint16 TRIPINPUT2:1;                
    Uint16 TRIPINPUT3:1;                
    Uint16 TRIPINPUT4:1;                
    Uint16 TRIPINPUT5:1;                
    Uint16 TRIPINPUT6:1;                
    Uint16 TRIPINPUT7:1;                
    Uint16 TRIPINPUT8:1;                
    Uint16 TRIPINPUT9:1;                
    Uint16 TRIPINPUT10:1;               
    Uint16 TRIPINPUT11:1;               
    Uint16 TRIPINPUT12:1;               
    Uint16 rsvd1:1;                     
    Uint16 TRIPINPUT14:1;               
    Uint16 TRIPINPUT15:1;               
    Uint16 rsvd2:1;                     
};

union DCBHTRIPSEL_REG {
    Uint16  all;
    struct  DCBHTRIPSEL_BITS  bit;
};

struct DCBLTRIPSEL_BITS {               
    Uint16 TRIPINPUT1:1;                
    Uint16 TRIPINPUT2:1;                
    Uint16 TRIPINPUT3:1;                
    Uint16 TRIPINPUT4:1;                
    Uint16 TRIPINPUT5:1;                
    Uint16 TRIPINPUT6:1;                
    Uint16 TRIPINPUT7:1;                
    Uint16 TRIPINPUT8:1;                
    Uint16 TRIPINPUT9:1;                
    Uint16 TRIPINPUT10:1;               
    Uint16 TRIPINPUT11:1;               
    Uint16 TRIPINPUT12:1;               
    Uint16 rsvd1:1;                     
    Uint16 TRIPINPUT14:1;               
    Uint16 TRIPINPUT15:1;               
    Uint16 rsvd2:1;                     
};

union DCBLTRIPSEL_REG {
    Uint16  all;
    struct  DCBLTRIPSEL_BITS  bit;
};

struct EPWMLOCK_BITS {                  
    Uint16 HRLOCK:1;                    
    Uint16 GLLOCK:1;                    
    Uint16 TZCFGLOCK:1;                 
    Uint16 TZCLRLOCK:1;                 
    Uint16 DCLOCK:1;                    
    Uint16 rsvd1:11;                    
    Uint16 KEY:16;                      
};

union EPWMLOCK_REG {
    Uint32  all;
    struct  EPWMLOCK_BITS  bit;
};

struct EPWM_REGS {
    union   TBCTL_REG                        TBCTL;                        
    union   TBCTL2_REG                       TBCTL2;                       
    Uint16                                   rsvd1;                        
    union   EPWMSYNCINSEL_REG                EPWMSYNCINSEL;                
    Uint16                                   TBCTR;                        
    union   TBSTS_REG                        TBSTS;                        
    union   EPWMSYNCOUTEN_REG                EPWMSYNCOUTEN;                
    union   TBCTL3_REG                       TBCTL3;                       
    union   CMPCTL_REG                       CMPCTL;                       
    union   CMPCTL2_REG                      CMPCTL2;                      
    Uint16                                   rsvd2[2];                     
    union   DBCTL_REG                        DBCTL;                        
    union   DBCTL2_REG                       DBCTL2;                       
    Uint16                                   rsvd3[2];                     
    union   AQCTL_REG                        AQCTL;                        
    union   AQTSRCSEL_REG                    AQTSRCSEL;                    
    Uint16                                   rsvd4[2];                     
    union   PCCTL_REG                        PCCTL;                        
    Uint16                                   rsvd5[3];                     
    union   VCAPCTL_REG                      VCAPCTL;                      
    union   VCNTCFG_REG                      VCNTCFG;                      
    Uint16                                   rsvd6[6];                     
    union   HRCNFG_REG                       HRCNFG;                       
    union   HRPWR_REG                        HRPWR;                        
    Uint16                                   rsvd7[4];                     
    union   HRMSTEP_REG                      HRMSTEP;                      
    union   HRCNFG2_REG                      HRCNFG2;                      
    Uint16                                   rsvd8[5];                     
    union   HRPCTL_REG                       HRPCTL;                       
    union   TRREM_REG                        TRREM;                        
    Uint16                                   rsvd9[5];                     
    union   GLDCTL_REG                       GLDCTL;                       
    union   GLDCFG_REG                       GLDCFG;                       
    Uint16                                   rsvd10[2];                    
    union   EPWMXLINK_REG                    EPWMXLINK;                    
    Uint16                                   rsvd11[6];                    
    union   AQCTLA_REG                       AQCTLA;                       
    union   AQCTLA2_REG                      AQCTLA2;                      
    union   AQCTLB_REG                       AQCTLB;                       
    union   AQCTLB2_REG                      AQCTLB2;                      
    Uint16                                   rsvd12[3];                    
    union   AQSFRC_REG                       AQSFRC;                       
    Uint16                                   rsvd13;                       
    union   AQCSFRC_REG                      AQCSFRC;                      
    Uint16                                   rsvd14[6];                    
    union   DBREDHR_REG                      DBREDHR;                      
    union   DBRED_REG                        DBRED;                        
    union   DBFEDHR_REG                      DBFEDHR;                      
    union   DBFED_REG                        DBFED;                        
    Uint16                                   rsvd15[12];                   
    union   TBPHS_REG                        TBPHS;                        
    Uint16                                   TBPRDHR;                      
    Uint16                                   TBPRD;                        
    Uint16                                   rsvd16[6];                    
    union   CMPA_REG                         CMPA;                         
    union   CMPB_REG                         CMPB;                         
    Uint16                                   rsvd17;                       
    Uint16                                   CMPC;                         
    Uint16                                   rsvd18;                       
    Uint16                                   CMPD;                         
    Uint16                                   rsvd19[2];                    
    union   GLDCTL2_REG                      GLDCTL2;                      
    Uint16                                   rsvd20[2];                    
    Uint16                                   SWVDELVAL;                    
    Uint16                                   rsvd21[8];                    
    union   TZSEL_REG                        TZSEL;                        
    Uint16                                   rsvd22;                       
    union   TZDCSEL_REG                      TZDCSEL;                      
    Uint16                                   rsvd23;                       
    union   TZCTL_REG                        TZCTL;                        
    union   TZCTL2_REG                       TZCTL2;                       
    union   TZCTLDCA_REG                     TZCTLDCA;                     
    union   TZCTLDCB_REG                     TZCTLDCB;                     
    Uint16                                   rsvd24[5];                    
    union   TZEINT_REG                       TZEINT;                       
    Uint16                                   rsvd25[5];                    
    union   TZFLG_REG                        TZFLG;                        
    union   TZCBCFLG_REG                     TZCBCFLG;                     
    union   TZOSTFLG_REG                     TZOSTFLG;                     
    Uint16                                   rsvd26;                       
    union   TZCLR_REG                        TZCLR;                        
    union   TZCBCCLR_REG                     TZCBCCLR;                     
    union   TZOSTCLR_REG                     TZOSTCLR;                     
    Uint16                                   rsvd27;                       
    union   TZFRC_REG                        TZFRC;                        
    Uint16                                   rsvd28[8];                    
    union   ETSEL_REG                        ETSEL;                        
    Uint16                                   rsvd29;                       
    union   ETPS_REG                         ETPS;                         
    Uint16                                   rsvd30;                       
    union   ETFLG_REG                        ETFLG;                        
    Uint16                                   rsvd31;                       
    union   ETCLR_REG                        ETCLR;                        
    Uint16                                   rsvd32;                       
    union   ETFRC_REG                        ETFRC;                        
    Uint16                                   rsvd33;                       
    union   ETINTPS_REG                      ETINTPS;                      
    Uint16                                   rsvd34;                       
    union   ETSOCPS_REG                      ETSOCPS;                      
    Uint16                                   rsvd35;                       
    union   ETCNTINITCTL_REG                 ETCNTINITCTL;                 
    Uint16                                   rsvd36;                       
    union   ETCNTINIT_REG                    ETCNTINIT;                    
    Uint16                                   rsvd37[11];                   
    union   DCTRIPSEL_REG                    DCTRIPSEL;                    
    Uint16                                   rsvd38[2];                    
    union   DCACTL_REG                       DCACTL;                       
    union   DCBCTL_REG                       DCBCTL;                       
    Uint16                                   rsvd39[2];                    
    union   DCFCTL_REG                       DCFCTL;                       
    union   DCCAPCTL_REG                     DCCAPCTL;                     
    Uint16                                   DCFOFFSET;                    
    Uint16                                   DCFOFFSETCNT;                 
    Uint16                                   DCFWINDOW;                    
    Uint16                                   DCFWINDOWCNT;                 
    union   BLANKPULSEMIXSEL_REG             BLANKPULSEMIXSEL;             
    Uint16                                   rsvd40;                       
    Uint16                                   DCCAP;                        
    Uint16                                   rsvd41[2];                    
    union   DCAHTRIPSEL_REG                  DCAHTRIPSEL;                  
    union   DCALTRIPSEL_REG                  DCALTRIPSEL;                  
    union   DCBHTRIPSEL_REG                  DCBHTRIPSEL;                  
    union   DCBLTRIPSEL_REG                  DCBLTRIPSEL;                  
    Uint16                                   rsvd42[36];                   
    union   EPWMLOCK_REG                     EPWMLOCK;                     
    Uint16                                   rsvd43;                       
    Uint16                                   HWVDELVAL;                    
    Uint16                                   VCNTVAL;                      
};




extern volatile struct EPWM_REGS EPwm1Regs;
extern volatile struct EPWM_REGS EPwm2Regs;
extern volatile struct EPWM_REGS EPwm3Regs;
extern volatile struct EPWM_REGS EPwm4Regs;
extern volatile struct EPWM_REGS EPwm5Regs;
extern volatile struct EPWM_REGS EPwm6Regs;
extern volatile struct EPWM_REGS EPwm7Regs;
extern volatile struct EPWM_REGS EPwm8Regs;



















































struct TRIP4MUX0TO15CFG_BITS {          
    Uint16 MUX0:2;                      
    Uint16 MUX1:2;                      
    Uint16 MUX2:2;                      
    Uint16 MUX3:2;                      
    Uint16 MUX4:2;                      
    Uint16 MUX5:2;                      
    Uint16 MUX6:2;                      
    Uint16 MUX7:2;                      
    Uint16 MUX8:2;                      
    Uint16 MUX9:2;                      
    Uint16 MUX10:2;                     
    Uint16 MUX11:2;                     
    Uint16 MUX12:2;                     
    Uint16 MUX13:2;                     
    Uint16 MUX14:2;                     
    Uint16 MUX15:2;                     
};

union TRIP4MUX0TO15CFG_REG {
    Uint32  all;
    struct  TRIP4MUX0TO15CFG_BITS  bit;
};

struct TRIP4MUX16TO31CFG_BITS {         
    Uint16 MUX16:2;                     
    Uint16 MUX17:2;                     
    Uint16 MUX18:2;                     
    Uint16 MUX19:2;                     
    Uint16 MUX20:2;                     
    Uint16 MUX21:2;                     
    Uint16 MUX22:2;                     
    Uint16 MUX23:2;                     
    Uint16 MUX24:2;                     
    Uint16 MUX25:2;                     
    Uint16 MUX26:2;                     
    Uint16 MUX27:2;                     
    Uint16 MUX28:2;                     
    Uint16 MUX29:2;                     
    Uint16 MUX30:2;                     
    Uint16 MUX31:2;                     
};

union TRIP4MUX16TO31CFG_REG {
    Uint32  all;
    struct  TRIP4MUX16TO31CFG_BITS  bit;
};

struct TRIP5MUX0TO15CFG_BITS {          
    Uint16 MUX0:2;                      
    Uint16 MUX1:2;                      
    Uint16 MUX2:2;                      
    Uint16 MUX3:2;                      
    Uint16 MUX4:2;                      
    Uint16 MUX5:2;                      
    Uint16 MUX6:2;                      
    Uint16 MUX7:2;                      
    Uint16 MUX8:2;                      
    Uint16 MUX9:2;                      
    Uint16 MUX10:2;                     
    Uint16 MUX11:2;                     
    Uint16 MUX12:2;                     
    Uint16 MUX13:2;                     
    Uint16 MUX14:2;                     
    Uint16 MUX15:2;                     
};

union TRIP5MUX0TO15CFG_REG {
    Uint32  all;
    struct  TRIP5MUX0TO15CFG_BITS  bit;
};

struct TRIP5MUX16TO31CFG_BITS {         
    Uint16 MUX16:2;                     
    Uint16 MUX17:2;                     
    Uint16 MUX18:2;                     
    Uint16 MUX19:2;                     
    Uint16 MUX20:2;                     
    Uint16 MUX21:2;                     
    Uint16 MUX22:2;                     
    Uint16 MUX23:2;                     
    Uint16 MUX24:2;                     
    Uint16 MUX25:2;                     
    Uint16 MUX26:2;                     
    Uint16 MUX27:2;                     
    Uint16 MUX28:2;                     
    Uint16 MUX29:2;                     
    Uint16 MUX30:2;                     
    Uint16 MUX31:2;                     
};

union TRIP5MUX16TO31CFG_REG {
    Uint32  all;
    struct  TRIP5MUX16TO31CFG_BITS  bit;
};

struct TRIP7MUX0TO15CFG_BITS {          
    Uint16 MUX0:2;                      
    Uint16 MUX1:2;                      
    Uint16 MUX2:2;                      
    Uint16 MUX3:2;                      
    Uint16 MUX4:2;                      
    Uint16 MUX5:2;                      
    Uint16 MUX6:2;                      
    Uint16 MUX7:2;                      
    Uint16 MUX8:2;                      
    Uint16 MUX9:2;                      
    Uint16 MUX10:2;                     
    Uint16 MUX11:2;                     
    Uint16 MUX12:2;                     
    Uint16 MUX13:2;                     
    Uint16 MUX14:2;                     
    Uint16 MUX15:2;                     
};

union TRIP7MUX0TO15CFG_REG {
    Uint32  all;
    struct  TRIP7MUX0TO15CFG_BITS  bit;
};

struct TRIP7MUX16TO31CFG_BITS {         
    Uint16 MUX16:2;                     
    Uint16 MUX17:2;                     
    Uint16 MUX18:2;                     
    Uint16 MUX19:2;                     
    Uint16 MUX20:2;                     
    Uint16 MUX21:2;                     
    Uint16 MUX22:2;                     
    Uint16 MUX23:2;                     
    Uint16 MUX24:2;                     
    Uint16 MUX25:2;                     
    Uint16 MUX26:2;                     
    Uint16 MUX27:2;                     
    Uint16 MUX28:2;                     
    Uint16 MUX29:2;                     
    Uint16 MUX30:2;                     
    Uint16 MUX31:2;                     
};

union TRIP7MUX16TO31CFG_REG {
    Uint32  all;
    struct  TRIP7MUX16TO31CFG_BITS  bit;
};

struct TRIP8MUX0TO15CFG_BITS {          
    Uint16 MUX0:2;                      
    Uint16 MUX1:2;                      
    Uint16 MUX2:2;                      
    Uint16 MUX3:2;                      
    Uint16 MUX4:2;                      
    Uint16 MUX5:2;                      
    Uint16 MUX6:2;                      
    Uint16 MUX7:2;                      
    Uint16 MUX8:2;                      
    Uint16 MUX9:2;                      
    Uint16 MUX10:2;                     
    Uint16 MUX11:2;                     
    Uint16 MUX12:2;                     
    Uint16 MUX13:2;                     
    Uint16 MUX14:2;                     
    Uint16 MUX15:2;                     
};

union TRIP8MUX0TO15CFG_REG {
    Uint32  all;
    struct  TRIP8MUX0TO15CFG_BITS  bit;
};

struct TRIP8MUX16TO31CFG_BITS {         
    Uint16 MUX16:2;                     
    Uint16 MUX17:2;                     
    Uint16 MUX18:2;                     
    Uint16 MUX19:2;                     
    Uint16 MUX20:2;                     
    Uint16 MUX21:2;                     
    Uint16 MUX22:2;                     
    Uint16 MUX23:2;                     
    Uint16 MUX24:2;                     
    Uint16 MUX25:2;                     
    Uint16 MUX26:2;                     
    Uint16 MUX27:2;                     
    Uint16 MUX28:2;                     
    Uint16 MUX29:2;                     
    Uint16 MUX30:2;                     
    Uint16 MUX31:2;                     
};

union TRIP8MUX16TO31CFG_REG {
    Uint32  all;
    struct  TRIP8MUX16TO31CFG_BITS  bit;
};

struct TRIP9MUX0TO15CFG_BITS {          
    Uint16 MUX0:2;                      
    Uint16 MUX1:2;                      
    Uint16 MUX2:2;                      
    Uint16 MUX3:2;                      
    Uint16 MUX4:2;                      
    Uint16 MUX5:2;                      
    Uint16 MUX6:2;                      
    Uint16 MUX7:2;                      
    Uint16 MUX8:2;                      
    Uint16 MUX9:2;                      
    Uint16 MUX10:2;                     
    Uint16 MUX11:2;                     
    Uint16 MUX12:2;                     
    Uint16 MUX13:2;                     
    Uint16 MUX14:2;                     
    Uint16 MUX15:2;                     
};

union TRIP9MUX0TO15CFG_REG {
    Uint32  all;
    struct  TRIP9MUX0TO15CFG_BITS  bit;
};

struct TRIP9MUX16TO31CFG_BITS {         
    Uint16 MUX16:2;                     
    Uint16 MUX17:2;                     
    Uint16 MUX18:2;                     
    Uint16 MUX19:2;                     
    Uint16 MUX20:2;                     
    Uint16 MUX21:2;                     
    Uint16 MUX22:2;                     
    Uint16 MUX23:2;                     
    Uint16 MUX24:2;                     
    Uint16 MUX25:2;                     
    Uint16 MUX26:2;                     
    Uint16 MUX27:2;                     
    Uint16 MUX28:2;                     
    Uint16 MUX29:2;                     
    Uint16 MUX30:2;                     
    Uint16 MUX31:2;                     
};

union TRIP9MUX16TO31CFG_REG {
    Uint32  all;
    struct  TRIP9MUX16TO31CFG_BITS  bit;
};

struct TRIP10MUX0TO15CFG_BITS {         
    Uint16 MUX0:2;                      
    Uint16 MUX1:2;                      
    Uint16 MUX2:2;                      
    Uint16 MUX3:2;                      
    Uint16 MUX4:2;                      
    Uint16 MUX5:2;                      
    Uint16 MUX6:2;                      
    Uint16 MUX7:2;                      
    Uint16 MUX8:2;                      
    Uint16 MUX9:2;                      
    Uint16 MUX10:2;                     
    Uint16 MUX11:2;                     
    Uint16 MUX12:2;                     
    Uint16 MUX13:2;                     
    Uint16 MUX14:2;                     
    Uint16 MUX15:2;                     
};

union TRIP10MUX0TO15CFG_REG {
    Uint32  all;
    struct  TRIP10MUX0TO15CFG_BITS  bit;
};

struct TRIP10MUX16TO31CFG_BITS {        
    Uint16 MUX16:2;                     
    Uint16 MUX17:2;                     
    Uint16 MUX18:2;                     
    Uint16 MUX19:2;                     
    Uint16 MUX20:2;                     
    Uint16 MUX21:2;                     
    Uint16 MUX22:2;                     
    Uint16 MUX23:2;                     
    Uint16 MUX24:2;                     
    Uint16 MUX25:2;                     
    Uint16 MUX26:2;                     
    Uint16 MUX27:2;                     
    Uint16 MUX28:2;                     
    Uint16 MUX29:2;                     
    Uint16 MUX30:2;                     
    Uint16 MUX31:2;                     
};

union TRIP10MUX16TO31CFG_REG {
    Uint32  all;
    struct  TRIP10MUX16TO31CFG_BITS  bit;
};

struct TRIP11MUX0TO15CFG_BITS {         
    Uint16 MUX0:2;                      
    Uint16 MUX1:2;                      
    Uint16 MUX2:2;                      
    Uint16 MUX3:2;                      
    Uint16 MUX4:2;                      
    Uint16 MUX5:2;                      
    Uint16 MUX6:2;                      
    Uint16 MUX7:2;                      
    Uint16 MUX8:2;                      
    Uint16 MUX9:2;                      
    Uint16 MUX10:2;                     
    Uint16 MUX11:2;                     
    Uint16 MUX12:2;                     
    Uint16 MUX13:2;                     
    Uint16 MUX14:2;                     
    Uint16 MUX15:2;                     
};

union TRIP11MUX0TO15CFG_REG {
    Uint32  all;
    struct  TRIP11MUX0TO15CFG_BITS  bit;
};

struct TRIP11MUX16TO31CFG_BITS {        
    Uint16 MUX16:2;                     
    Uint16 MUX17:2;                     
    Uint16 MUX18:2;                     
    Uint16 MUX19:2;                     
    Uint16 MUX20:2;                     
    Uint16 MUX21:2;                     
    Uint16 MUX22:2;                     
    Uint16 MUX23:2;                     
    Uint16 MUX24:2;                     
    Uint16 MUX25:2;                     
    Uint16 MUX26:2;                     
    Uint16 MUX27:2;                     
    Uint16 MUX28:2;                     
    Uint16 MUX29:2;                     
    Uint16 MUX30:2;                     
    Uint16 MUX31:2;                     
};

union TRIP11MUX16TO31CFG_REG {
    Uint32  all;
    struct  TRIP11MUX16TO31CFG_BITS  bit;
};

struct TRIP12MUX0TO15CFG_BITS {         
    Uint16 MUX0:2;                      
    Uint16 MUX1:2;                      
    Uint16 MUX2:2;                      
    Uint16 MUX3:2;                      
    Uint16 MUX4:2;                      
    Uint16 MUX5:2;                      
    Uint16 MUX6:2;                      
    Uint16 MUX7:2;                      
    Uint16 MUX8:2;                      
    Uint16 MUX9:2;                      
    Uint16 MUX10:2;                     
    Uint16 MUX11:2;                     
    Uint16 MUX12:2;                     
    Uint16 MUX13:2;                     
    Uint16 MUX14:2;                     
    Uint16 MUX15:2;                     
};

union TRIP12MUX0TO15CFG_REG {
    Uint32  all;
    struct  TRIP12MUX0TO15CFG_BITS  bit;
};

struct TRIP12MUX16TO31CFG_BITS {        
    Uint16 MUX16:2;                     
    Uint16 MUX17:2;                     
    Uint16 MUX18:2;                     
    Uint16 MUX19:2;                     
    Uint16 MUX20:2;                     
    Uint16 MUX21:2;                     
    Uint16 MUX22:2;                     
    Uint16 MUX23:2;                     
    Uint16 MUX24:2;                     
    Uint16 MUX25:2;                     
    Uint16 MUX26:2;                     
    Uint16 MUX27:2;                     
    Uint16 MUX28:2;                     
    Uint16 MUX29:2;                     
    Uint16 MUX30:2;                     
    Uint16 MUX31:2;                     
};

union TRIP12MUX16TO31CFG_REG {
    Uint32  all;
    struct  TRIP12MUX16TO31CFG_BITS  bit;
};

struct TRIP4MUXENABLE_BITS {            
    Uint16 MUX0:1;                      
    Uint16 MUX1:1;                      
    Uint16 MUX2:1;                      
    Uint16 MUX3:1;                      
    Uint16 MUX4:1;                      
    Uint16 MUX5:1;                      
    Uint16 MUX6:1;                      
    Uint16 MUX7:1;                      
    Uint16 MUX8:1;                      
    Uint16 MUX9:1;                      
    Uint16 MUX10:1;                     
    Uint16 MUX11:1;                     
    Uint16 MUX12:1;                     
    Uint16 MUX13:1;                     
    Uint16 MUX14:1;                     
    Uint16 MUX15:1;                     
    Uint16 MUX16:1;                     
    Uint16 MUX17:1;                     
    Uint16 MUX18:1;                     
    Uint16 MUX19:1;                     
    Uint16 MUX20:1;                     
    Uint16 MUX21:1;                     
    Uint16 MUX22:1;                     
    Uint16 MUX23:1;                     
    Uint16 MUX24:1;                     
    Uint16 MUX25:1;                     
    Uint16 MUX26:1;                     
    Uint16 MUX27:1;                     
    Uint16 MUX28:1;                     
    Uint16 MUX29:1;                     
    Uint16 MUX30:1;                     
    Uint16 MUX31:1;                     
};

union TRIP4MUXENABLE_REG {
    Uint32  all;
    struct  TRIP4MUXENABLE_BITS  bit;
};

struct TRIP5MUXENABLE_BITS {            
    Uint16 MUX0:1;                      
    Uint16 MUX1:1;                      
    Uint16 MUX2:1;                      
    Uint16 MUX3:1;                      
    Uint16 MUX4:1;                      
    Uint16 MUX5:1;                      
    Uint16 MUX6:1;                      
    Uint16 MUX7:1;                      
    Uint16 MUX8:1;                      
    Uint16 MUX9:1;                      
    Uint16 MUX10:1;                     
    Uint16 MUX11:1;                     
    Uint16 MUX12:1;                     
    Uint16 MUX13:1;                     
    Uint16 MUX14:1;                     
    Uint16 MUX15:1;                     
    Uint16 MUX16:1;                     
    Uint16 MUX17:1;                     
    Uint16 MUX18:1;                     
    Uint16 MUX19:1;                     
    Uint16 MUX20:1;                     
    Uint16 MUX21:1;                     
    Uint16 MUX22:1;                     
    Uint16 MUX23:1;                     
    Uint16 MUX24:1;                     
    Uint16 MUX25:1;                     
    Uint16 MUX26:1;                     
    Uint16 MUX27:1;                     
    Uint16 MUX28:1;                     
    Uint16 MUX29:1;                     
    Uint16 MUX30:1;                     
    Uint16 MUX31:1;                     
};

union TRIP5MUXENABLE_REG {
    Uint32  all;
    struct  TRIP5MUXENABLE_BITS  bit;
};

struct TRIP7MUXENABLE_BITS {            
    Uint16 MUX0:1;                      
    Uint16 MUX1:1;                      
    Uint16 MUX2:1;                      
    Uint16 MUX3:1;                      
    Uint16 MUX4:1;                      
    Uint16 MUX5:1;                      
    Uint16 MUX6:1;                      
    Uint16 MUX7:1;                      
    Uint16 MUX8:1;                      
    Uint16 MUX9:1;                      
    Uint16 MUX10:1;                     
    Uint16 MUX11:1;                     
    Uint16 MUX12:1;                     
    Uint16 MUX13:1;                     
    Uint16 MUX14:1;                     
    Uint16 MUX15:1;                     
    Uint16 MUX16:1;                     
    Uint16 MUX17:1;                     
    Uint16 MUX18:1;                     
    Uint16 MUX19:1;                     
    Uint16 MUX20:1;                     
    Uint16 MUX21:1;                     
    Uint16 MUX22:1;                     
    Uint16 MUX23:1;                     
    Uint16 MUX24:1;                     
    Uint16 MUX25:1;                     
    Uint16 MUX26:1;                     
    Uint16 MUX27:1;                     
    Uint16 MUX28:1;                     
    Uint16 MUX29:1;                     
    Uint16 MUX30:1;                     
    Uint16 MUX31:1;                     
};

union TRIP7MUXENABLE_REG {
    Uint32  all;
    struct  TRIP7MUXENABLE_BITS  bit;
};

struct TRIP8MUXENABLE_BITS {            
    Uint16 MUX0:1;                      
    Uint16 MUX1:1;                      
    Uint16 MUX2:1;                      
    Uint16 MUX3:1;                      
    Uint16 MUX4:1;                      
    Uint16 MUX5:1;                      
    Uint16 MUX6:1;                      
    Uint16 MUX7:1;                      
    Uint16 MUX8:1;                      
    Uint16 MUX9:1;                      
    Uint16 MUX10:1;                     
    Uint16 MUX11:1;                     
    Uint16 MUX12:1;                     
    Uint16 MUX13:1;                     
    Uint16 MUX14:1;                     
    Uint16 MUX15:1;                     
    Uint16 MUX16:1;                     
    Uint16 MUX17:1;                     
    Uint16 MUX18:1;                     
    Uint16 MUX19:1;                     
    Uint16 MUX20:1;                     
    Uint16 MUX21:1;                     
    Uint16 MUX22:1;                     
    Uint16 MUX23:1;                     
    Uint16 MUX24:1;                     
    Uint16 MUX25:1;                     
    Uint16 MUX26:1;                     
    Uint16 MUX27:1;                     
    Uint16 MUX28:1;                     
    Uint16 MUX29:1;                     
    Uint16 MUX30:1;                     
    Uint16 MUX31:1;                     
};

union TRIP8MUXENABLE_REG {
    Uint32  all;
    struct  TRIP8MUXENABLE_BITS  bit;
};

struct TRIP9MUXENABLE_BITS {            
    Uint16 MUX0:1;                      
    Uint16 MUX1:1;                      
    Uint16 MUX2:1;                      
    Uint16 MUX3:1;                      
    Uint16 MUX4:1;                      
    Uint16 MUX5:1;                      
    Uint16 MUX6:1;                      
    Uint16 MUX7:1;                      
    Uint16 MUX8:1;                      
    Uint16 MUX9:1;                      
    Uint16 MUX10:1;                     
    Uint16 MUX11:1;                     
    Uint16 MUX12:1;                     
    Uint16 MUX13:1;                     
    Uint16 MUX14:1;                     
    Uint16 MUX15:1;                     
    Uint16 MUX16:1;                     
    Uint16 MUX17:1;                     
    Uint16 MUX18:1;                     
    Uint16 MUX19:1;                     
    Uint16 MUX20:1;                     
    Uint16 MUX21:1;                     
    Uint16 MUX22:1;                     
    Uint16 MUX23:1;                     
    Uint16 MUX24:1;                     
    Uint16 MUX25:1;                     
    Uint16 MUX26:1;                     
    Uint16 MUX27:1;                     
    Uint16 MUX28:1;                     
    Uint16 MUX29:1;                     
    Uint16 MUX30:1;                     
    Uint16 MUX31:1;                     
};

union TRIP9MUXENABLE_REG {
    Uint32  all;
    struct  TRIP9MUXENABLE_BITS  bit;
};

struct TRIP10MUXENABLE_BITS {           
    Uint16 MUX0:1;                      
    Uint16 MUX1:1;                      
    Uint16 MUX2:1;                      
    Uint16 MUX3:1;                      
    Uint16 MUX4:1;                      
    Uint16 MUX5:1;                      
    Uint16 MUX6:1;                      
    Uint16 MUX7:1;                      
    Uint16 MUX8:1;                      
    Uint16 MUX9:1;                      
    Uint16 MUX10:1;                     
    Uint16 MUX11:1;                     
    Uint16 MUX12:1;                     
    Uint16 MUX13:1;                     
    Uint16 MUX14:1;                     
    Uint16 MUX15:1;                     
    Uint16 MUX16:1;                     
    Uint16 MUX17:1;                     
    Uint16 MUX18:1;                     
    Uint16 MUX19:1;                     
    Uint16 MUX20:1;                     
    Uint16 MUX21:1;                     
    Uint16 MUX22:1;                     
    Uint16 MUX23:1;                     
    Uint16 MUX24:1;                     
    Uint16 MUX25:1;                     
    Uint16 MUX26:1;                     
    Uint16 MUX27:1;                     
    Uint16 MUX28:1;                     
    Uint16 MUX29:1;                     
    Uint16 MUX30:1;                     
    Uint16 MUX31:1;                     
};

union TRIP10MUXENABLE_REG {
    Uint32  all;
    struct  TRIP10MUXENABLE_BITS  bit;
};

struct TRIP11MUXENABLE_BITS {           
    Uint16 MUX0:1;                      
    Uint16 MUX1:1;                      
    Uint16 MUX2:1;                      
    Uint16 MUX3:1;                      
    Uint16 MUX4:1;                      
    Uint16 MUX5:1;                      
    Uint16 MUX6:1;                      
    Uint16 MUX7:1;                      
    Uint16 MUX8:1;                      
    Uint16 MUX9:1;                      
    Uint16 MUX10:1;                     
    Uint16 MUX11:1;                     
    Uint16 MUX12:1;                     
    Uint16 MUX13:1;                     
    Uint16 MUX14:1;                     
    Uint16 MUX15:1;                     
    Uint16 MUX16:1;                     
    Uint16 MUX17:1;                     
    Uint16 MUX18:1;                     
    Uint16 MUX19:1;                     
    Uint16 MUX20:1;                     
    Uint16 MUX21:1;                     
    Uint16 MUX22:1;                     
    Uint16 MUX23:1;                     
    Uint16 MUX24:1;                     
    Uint16 MUX25:1;                     
    Uint16 MUX26:1;                     
    Uint16 MUX27:1;                     
    Uint16 MUX28:1;                     
    Uint16 MUX29:1;                     
    Uint16 MUX30:1;                     
    Uint16 MUX31:1;                     
};

union TRIP11MUXENABLE_REG {
    Uint32  all;
    struct  TRIP11MUXENABLE_BITS  bit;
};

struct TRIP12MUXENABLE_BITS {           
    Uint16 MUX0:1;                      
    Uint16 MUX1:1;                      
    Uint16 MUX2:1;                      
    Uint16 MUX3:1;                      
    Uint16 MUX4:1;                      
    Uint16 MUX5:1;                      
    Uint16 MUX6:1;                      
    Uint16 MUX7:1;                      
    Uint16 MUX8:1;                      
    Uint16 MUX9:1;                      
    Uint16 MUX10:1;                     
    Uint16 MUX11:1;                     
    Uint16 MUX12:1;                     
    Uint16 MUX13:1;                     
    Uint16 MUX14:1;                     
    Uint16 MUX15:1;                     
    Uint16 MUX16:1;                     
    Uint16 MUX17:1;                     
    Uint16 MUX18:1;                     
    Uint16 MUX19:1;                     
    Uint16 MUX20:1;                     
    Uint16 MUX21:1;                     
    Uint16 MUX22:1;                     
    Uint16 MUX23:1;                     
    Uint16 MUX24:1;                     
    Uint16 MUX25:1;                     
    Uint16 MUX26:1;                     
    Uint16 MUX27:1;                     
    Uint16 MUX28:1;                     
    Uint16 MUX29:1;                     
    Uint16 MUX30:1;                     
    Uint16 MUX31:1;                     
};

union TRIP12MUXENABLE_REG {
    Uint32  all;
    struct  TRIP12MUXENABLE_BITS  bit;
};

struct TRIPOUTINV_BITS {                
    Uint16 TRIP4:1;                     
    Uint16 TRIP5:1;                     
    Uint16 TRIP7:1;                     
    Uint16 TRIP8:1;                     
    Uint16 TRIP9:1;                     
    Uint16 TRIP10:1;                    
    Uint16 TRIP11:1;                    
    Uint16 TRIP12:1;                    
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union TRIPOUTINV_REG {
    Uint32  all;
    struct  TRIPOUTINV_BITS  bit;
};

struct TRIPLOCK_BITS {                  
    Uint16 LOCK:1;                      
    Uint16 rsvd1:15;                    
    Uint16 KEY:16;                      
};

union TRIPLOCK_REG {
    Uint32  all;
    struct  TRIPLOCK_BITS  bit;
};

struct EPWM_XBAR_REGS {
    union   TRIP4MUX0TO15CFG_REG             TRIP4MUX0TO15CFG;             
    union   TRIP4MUX16TO31CFG_REG            TRIP4MUX16TO31CFG;            
    union   TRIP5MUX0TO15CFG_REG             TRIP5MUX0TO15CFG;             
    union   TRIP5MUX16TO31CFG_REG            TRIP5MUX16TO31CFG;            
    union   TRIP7MUX0TO15CFG_REG             TRIP7MUX0TO15CFG;             
    union   TRIP7MUX16TO31CFG_REG            TRIP7MUX16TO31CFG;            
    union   TRIP8MUX0TO15CFG_REG             TRIP8MUX0TO15CFG;             
    union   TRIP8MUX16TO31CFG_REG            TRIP8MUX16TO31CFG;            
    union   TRIP9MUX0TO15CFG_REG             TRIP9MUX0TO15CFG;             
    union   TRIP9MUX16TO31CFG_REG            TRIP9MUX16TO31CFG;            
    union   TRIP10MUX0TO15CFG_REG            TRIP10MUX0TO15CFG;            
    union   TRIP10MUX16TO31CFG_REG           TRIP10MUX16TO31CFG;           
    union   TRIP11MUX0TO15CFG_REG            TRIP11MUX0TO15CFG;            
    union   TRIP11MUX16TO31CFG_REG           TRIP11MUX16TO31CFG;           
    union   TRIP12MUX0TO15CFG_REG            TRIP12MUX0TO15CFG;            
    union   TRIP12MUX16TO31CFG_REG           TRIP12MUX16TO31CFG;           
    union   TRIP4MUXENABLE_REG               TRIP4MUXENABLE;               
    union   TRIP5MUXENABLE_REG               TRIP5MUXENABLE;               
    union   TRIP7MUXENABLE_REG               TRIP7MUXENABLE;               
    union   TRIP8MUXENABLE_REG               TRIP8MUXENABLE;               
    union   TRIP9MUXENABLE_REG               TRIP9MUXENABLE;               
    union   TRIP10MUXENABLE_REG              TRIP10MUXENABLE;              
    union   TRIP11MUXENABLE_REG              TRIP11MUXENABLE;              
    union   TRIP12MUXENABLE_REG              TRIP12MUXENABLE;              
    Uint16                                   rsvd1[8];                     
    union   TRIPOUTINV_REG                   TRIPOUTINV;                   
    Uint16                                   rsvd2[4];                     
    union   TRIPLOCK_REG                     TRIPLOCK;                     
};




extern volatile struct EPWM_XBAR_REGS EPwmXbarRegs;



















































struct QDECCTL_BITS {                   
    Uint16 QIDIRE:1;                    
    Uint16 rsvd1:4;                     
    Uint16 QSP:1;                       
    Uint16 QIP:1;                       
    Uint16 QBP:1;                       
    Uint16 QAP:1;                       
    Uint16 IGATE:1;                     
    Uint16 SWAP:1;                      
    Uint16 XCR:1;                       
    Uint16 SPSEL:1;                     
    Uint16 SOEN:1;                      
    Uint16 QSRC:2;                      
};

union QDECCTL_REG {
    Uint16  all;
    struct  QDECCTL_BITS  bit;
};

struct QEPCTL_BITS {                    
    Uint16 WDE:1;                       
    Uint16 UTE:1;                       
    Uint16 QCLM:1;                      
    Uint16 QPEN:1;                      
    Uint16 IEL:2;                       
    Uint16 SEL:1;                       
    Uint16 SWI:1;                       
    Uint16 IEI:2;                       
    Uint16 SEI:2;                       
    Uint16 PCRM:2;                      
    Uint16 FREE_SOFT:2;                 
};

union QEPCTL_REG {
    Uint16  all;
    struct  QEPCTL_BITS  bit;
};

struct QCAPCTL_BITS {                   
    Uint16 UPPS:4;                      
    Uint16 CCPS:3;                      
    Uint16 rsvd1:8;                     
    Uint16 CEN:1;                       
};

union QCAPCTL_REG {
    Uint16  all;
    struct  QCAPCTL_BITS  bit;
};

struct QPOSCTL_BITS {                   
    Uint16 PCSPW:12;                    
    Uint16 PCE:1;                       
    Uint16 PCPOL:1;                     
    Uint16 PCLOAD:1;                    
    Uint16 PCSHDW:1;                    
};

union QPOSCTL_REG {
    Uint16  all;
    struct  QPOSCTL_BITS  bit;
};

struct QEINT_BITS {                     
    Uint16 rsvd1:1;                     
    Uint16 PCE:1;                       
    Uint16 QPE:1;                       
    Uint16 QDC:1;                       
    Uint16 WTO:1;                       
    Uint16 PCU:1;                       
    Uint16 PCO:1;                       
    Uint16 PCR:1;                       
    Uint16 PCM:1;                       
    Uint16 SEL:1;                       
    Uint16 IEL:1;                       
    Uint16 UTO:1;                       
    Uint16 QMAE:1;                      
    Uint16 rsvd2:3;                     
};

union QEINT_REG {
    Uint16  all;
    struct  QEINT_BITS  bit;
};

struct QFLG_BITS {                      
    Uint16 INT:1;                       
    Uint16 PCE:1;                       
    Uint16 PHE:1;                       
    Uint16 QDC:1;                       
    Uint16 WTO:1;                       
    Uint16 PCU:1;                       
    Uint16 PCO:1;                       
    Uint16 PCR:1;                       
    Uint16 PCM:1;                       
    Uint16 SEL:1;                       
    Uint16 IEL:1;                       
    Uint16 UTO:1;                       
    Uint16 QMAE:1;                      
    Uint16 rsvd1:3;                     
};

union QFLG_REG {
    Uint16  all;
    struct  QFLG_BITS  bit;
};

struct QCLR_BITS {                      
    Uint16 INT:1;                       
    Uint16 PCE:1;                       
    Uint16 PHE:1;                       
    Uint16 QDC:1;                       
    Uint16 WTO:1;                       
    Uint16 PCU:1;                       
    Uint16 PCO:1;                       
    Uint16 PCR:1;                       
    Uint16 PCM:1;                       
    Uint16 SEL:1;                       
    Uint16 IEL:1;                       
    Uint16 UTO:1;                       
    Uint16 QMAE:1;                      
    Uint16 rsvd1:3;                     
};

union QCLR_REG {
    Uint16  all;
    struct  QCLR_BITS  bit;
};

struct QFRC_BITS {                      
    Uint16 rsvd1:1;                     
    Uint16 PCE:1;                       
    Uint16 PHE:1;                       
    Uint16 QDC:1;                       
    Uint16 WTO:1;                       
    Uint16 PCU:1;                       
    Uint16 PCO:1;                       
    Uint16 PCR:1;                       
    Uint16 PCM:1;                       
    Uint16 SEL:1;                       
    Uint16 IEL:1;                       
    Uint16 UTO:1;                       
    Uint16 QMAE:1;                      
    Uint16 rsvd2:3;                     
};

union QFRC_REG {
    Uint16  all;
    struct  QFRC_BITS  bit;
};

struct QEPSTS_BITS {                    
    Uint16 PCEF:1;                      
    Uint16 FIMF:1;                      
    Uint16 CDEF:1;                      
    Uint16 COEF:1;                      
    Uint16 QDLF:1;                      
    Uint16 QDF:1;                       
    Uint16 FIDF:1;                      
    Uint16 UPEVNT:1;                    
    Uint16 rsvd1:8;                     
};

union QEPSTS_REG {
    Uint16  all;
    struct  QEPSTS_BITS  bit;
};

struct REV_BITS {                       
    Uint16 MAJOR:3;                     
    Uint16 MINOR:3;                     
    Uint16 rsvd1:10;                    
    Uint16 rsvd2:16;                    
};

union REV_REG {
    Uint32  all;
    struct  REV_BITS  bit;
};

struct QEPSTROBESEL_BITS {              
    Uint16 STROBESEL:2;                 
    Uint16 rsvd1:14;                    
    Uint16 rsvd2:16;                    
};

union QEPSTROBESEL_REG {
    Uint32  all;
    struct  QEPSTROBESEL_BITS  bit;
};

struct QMACTRL_BITS {                   
    Uint16 MODE:3;                      
    Uint16 rsvd1:13;                    
    Uint16 rsvd2:16;                    
};

union QMACTRL_REG {
    Uint32  all;
    struct  QMACTRL_BITS  bit;
};

struct QEPSRCSEL_BITS {                 
    Uint16 QEPASEL:4;                   
    Uint16 QEPBSEL:4;                   
    Uint16 QEPISEL:4;                   
    Uint16 QEPSSEL:4;                   
    Uint16 rsvd1:16;                    
};

union QEPSRCSEL_REG {
    Uint32  all;
    struct  QEPSRCSEL_BITS  bit;
};

struct EQEP_REGS {
    Uint32                                   QPOSCNT;                      
    Uint32                                   QPOSINIT;                     
    Uint32                                   QPOSMAX;                      
    Uint32                                   QPOSCMP;                      
    Uint32                                   QPOSILAT;                     
    Uint32                                   QPOSSLAT;                     
    Uint32                                   QPOSLAT;                      
    Uint32                                   QUTMR;                        
    Uint32                                   QUPRD;                        
    Uint16                                   QWDTMR;                       
    Uint16                                   QWDPRD;                       
    union   QDECCTL_REG                      QDECCTL;                      
    union   QEPCTL_REG                       QEPCTL;                       
    union   QCAPCTL_REG                      QCAPCTL;                      
    union   QPOSCTL_REG                      QPOSCTL;                      
    union   QEINT_REG                        QEINT;                        
    union   QFLG_REG                         QFLG;                         
    union   QCLR_REG                         QCLR;                         
    union   QFRC_REG                         QFRC;                         
    union   QEPSTS_REG                       QEPSTS;                       
    Uint16                                   QCTMR;                        
    Uint16                                   QCPRD;                        
    Uint16                                   QCTMRLAT;                     
    Uint16                                   QCPRDLAT;                     
    Uint16                                   rsvd1[15];                    
    union   REV_REG                          REV;                          
    union   QEPSTROBESEL_REG                 QEPSTROBESEL;                 
    union   QMACTRL_REG                      QMACTRL;                      
    union   QEPSRCSEL_REG                    QEPSRCSEL;                    
};




extern volatile struct EQEP_REGS EQep1Regs;
extern volatile struct EQEP_REGS EQep2Regs;



















































struct GLBL_EVENT_STAT_BITS {           
    Uint16 HWBP1:1;                     
    Uint16 HWBP2:1;                     
    Uint16 HWBP3:1;                     
    Uint16 HWBP4:1;                     
    Uint16 HWBP5:1;                     
    Uint16 HWBP6:1;                     
    Uint16 HWBP7:1;                     
    Uint16 HWBP8:1;                     
    Uint16 CTM1:1;                      
    Uint16 CTM2:1;                      
    Uint16 CTM3:1;                      
    Uint16 CTM4:1;                      
    Uint16 rsvd1:4;                     
};

union GLBL_EVENT_STAT_REG {
    Uint16  all;
    struct  GLBL_EVENT_STAT_BITS  bit;
};

struct GLBL_HALT_STAT_BITS {            
    Uint16 HWBP1:1;                     
    Uint16 HWBP2:1;                     
    Uint16 HWBP3:1;                     
    Uint16 HWBP4:1;                     
    Uint16 HWBP5:1;                     
    Uint16 HWBP6:1;                     
    Uint16 HWBP7:1;                     
    Uint16 HWBP8:1;                     
    Uint16 CTM1:1;                      
    Uint16 CTM2:1;                      
    Uint16 CTM3:1;                      
    Uint16 CTM4:1;                      
    Uint16 rsvd1:4;                     
};

union GLBL_HALT_STAT_REG {
    Uint16  all;
    struct  GLBL_HALT_STAT_BITS  bit;
};

struct GLBL_ENABLE_BITS {               
    Uint16 HWBP1:1;                     
    Uint16 HWBP2:1;                     
    Uint16 HWBP3:1;                     
    Uint16 HWBP4:1;                     
    Uint16 HWBP5:1;                     
    Uint16 HWBP6:1;                     
    Uint16 HWBP7:1;                     
    Uint16 HWBP8:1;                     
    Uint16 CTM1:1;                      
    Uint16 CTM2:1;                      
    Uint16 CTM3:1;                      
    Uint16 CTM4:1;                      
    Uint16 rsvd1:4;                     
};

union GLBL_ENABLE_REG {
    Uint16  all;
    struct  GLBL_ENABLE_BITS  bit;
};

struct GLBL_CTM_RESET_BITS {            
    Uint16 CTM1:1;                      
    Uint16 CTM2:1;                      
    Uint16 CTM3:1;                      
    Uint16 CTM4:1;                      
    Uint16 rsvd1:12;                    
};

union GLBL_CTM_RESET_REG {
    Uint16  all;
    struct  GLBL_CTM_RESET_BITS  bit;
};

struct GLBL_NMI_CTL_BITS {              
    Uint16 HWBP1:1;                     
    Uint16 HWBP2:1;                     
    Uint16 HWBP3:1;                     
    Uint16 HWBP4:1;                     
    Uint16 HWBP5:1;                     
    Uint16 HWBP6:1;                     
    Uint16 HWBP7:1;                     
    Uint16 HWBP8:1;                     
    Uint16 CTM1:1;                      
    Uint16 CTM2:1;                      
    Uint16 CTM3:1;                      
    Uint16 CTM4:1;                      
    Uint16 rsvd1:4;                     
};

union GLBL_NMI_CTL_REG {
    Uint16  all;
    struct  GLBL_NMI_CTL_BITS  bit;
};

struct GLBL_OWNER_BITS {                
    Uint16 OWNER:2;                     
    Uint16 rsvd1:14;                    
};

union GLBL_OWNER_REG {
    Uint16  all;
    struct  GLBL_OWNER_BITS  bit;
};

struct GLBL_EVENT_AND_MASK_BITS {       
    Uint16 MASK1_HWBP1:1;               
    Uint16 MASK1_HWBP2:1;               
    Uint16 MASK1_HWBP3:1;               
    Uint16 MASK1_HWBP4:1;               
    Uint16 MASK1_HWBP5:1;               
    Uint16 MASK1_HWBP6:1;               
    Uint16 MASK1_HWBP7:1;               
    Uint16 MASK1_HWBP8:1;               
    Uint16 MASK2_HWBP1:1;               
    Uint16 MASK2_HWBP2:1;               
    Uint16 MASK2_HWBP3:1;               
    Uint16 MASK2_HWBP4:1;               
    Uint16 MASK2_HWBP5:1;               
    Uint16 MASK2_HWBP6:1;               
    Uint16 MASK2_HWBP7:1;               
    Uint16 MASK2_HWBP8:1;               
    Uint16 MASK3_HWBP1:1;               
    Uint16 MASK3_HWBP2:1;               
    Uint16 MASK3_HWBP3:1;               
    Uint16 MASK3_HWBP4:1;               
    Uint16 MASK3_HWBP5:1;               
    Uint16 MASK3_HWBP6:1;               
    Uint16 MASK3_HWBP7:1;               
    Uint16 MASK3_HWBP8:1;               
    Uint16 MASK4_HWBP1:1;               
    Uint16 MASK4_HWBP2:1;               
    Uint16 MASK4_HWBP3:1;               
    Uint16 MASK4_HWBP4:1;               
    Uint16 MASK4_HWBP5:1;               
    Uint16 MASK4_HWBP6:1;               
    Uint16 MASK4_HWBP7:1;               
    Uint16 MASK4_HWBP8:1;               
};

union GLBL_EVENT_AND_MASK_REG {
    Uint32  all;
    struct  GLBL_EVENT_AND_MASK_BITS  bit;
};

struct GLBL_EVENT_OR_MASK_BITS {        
    Uint16 MASK1_HWBP1:1;               
    Uint16 MASK1_HWBP2:1;               
    Uint16 MASK1_HWBP3:1;               
    Uint16 MASK1_HWBP4:1;               
    Uint16 MASK1_HWBP5:1;               
    Uint16 MASK1_HWBP6:1;               
    Uint16 MASK1_HWBP7:1;               
    Uint16 MASK1_HWBP8:1;               
    Uint16 MASK2_HWBP1:1;               
    Uint16 MASK2_HWBP2:1;               
    Uint16 MASK2_HWBP3:1;               
    Uint16 MASK2_HWBP4:1;               
    Uint16 MASK2_HWBP5:1;               
    Uint16 MASK2_HWBP6:1;               
    Uint16 MASK2_HWBP7:1;               
    Uint16 MASK2_HWBP8:1;               
    Uint16 MASK3_HWBP1:1;               
    Uint16 MASK3_HWBP2:1;               
    Uint16 MASK3_HWBP3:1;               
    Uint16 MASK3_HWBP4:1;               
    Uint16 MASK3_HWBP5:1;               
    Uint16 MASK3_HWBP6:1;               
    Uint16 MASK3_HWBP7:1;               
    Uint16 MASK3_HWBP8:1;               
    Uint16 MASK4_HWBP1:1;               
    Uint16 MASK4_HWBP2:1;               
    Uint16 MASK4_HWBP3:1;               
    Uint16 MASK4_HWBP4:1;               
    Uint16 MASK4_HWBP5:1;               
    Uint16 MASK4_HWBP6:1;               
    Uint16 MASK4_HWBP7:1;               
    Uint16 MASK4_HWBP8:1;               
};

union GLBL_EVENT_OR_MASK_REG {
    Uint32  all;
    struct  GLBL_EVENT_OR_MASK_BITS  bit;
};

struct GLBL_AND_EVENT_INT_MASK_BITS {   
    Uint16 RTOSINT_MASK1:1;             
    Uint16 RTOSINT_MASK2:1;             
    Uint16 RTOSINT_MASK3:1;             
    Uint16 RTOSINT_MASK4:1;             
    Uint16 rsvd1:12;                    
};

union GLBL_AND_EVENT_INT_MASK_REG {
    Uint16  all;
    struct  GLBL_AND_EVENT_INT_MASK_BITS  bit;
};

struct GLBL_OR_EVENT_INT_MASK_BITS {    
    Uint16 RTOSINT_MASK1:1;             
    Uint16 RTOSINT_MASK2:1;             
    Uint16 RTOSINT_MASK3:1;             
    Uint16 RTOSINT_MASK4:1;             
    Uint16 rsvd1:12;                    
};

union GLBL_OR_EVENT_INT_MASK_REG {
    Uint16  all;
    struct  GLBL_OR_EVENT_INT_MASK_BITS  bit;
};

struct ERAD_GLOBAL_REGS {
    union   GLBL_EVENT_STAT_REG              GLBL_EVENT_STAT;              
    Uint16                                   rsvd1;                        
    union   GLBL_HALT_STAT_REG               GLBL_HALT_STAT;               
    Uint16                                   rsvd2;                        
    union   GLBL_ENABLE_REG                  GLBL_ENABLE;                  
    Uint16                                   rsvd3;                        
    union   GLBL_CTM_RESET_REG               GLBL_CTM_RESET;               
    Uint16                                   rsvd4;                        
    union   GLBL_NMI_CTL_REG                 GLBL_NMI_CTL;                 
    Uint16                                   rsvd5;                        
    union   GLBL_OWNER_REG                   GLBL_OWNER;                   
    Uint16                                   rsvd6;                        
    union   GLBL_EVENT_AND_MASK_REG          GLBL_EVENT_AND_MASK;          
    union   GLBL_EVENT_OR_MASK_REG           GLBL_EVENT_OR_MASK;           
    union   GLBL_AND_EVENT_INT_MASK_REG      GLBL_AND_EVENT_INT_MASK;      
    Uint16                                   rsvd7;                        
    union   GLBL_OR_EVENT_INT_MASK_REG       GLBL_OR_EVENT_INT_MASK;       
};

struct HWBP_CLEAR_BITS {                
    Uint16 EVENT_CLR:1;                 
    Uint16 rsvd1:15;                    
};

union HWBP_CLEAR_REG {
    Uint16  all;
    struct  HWBP_CLEAR_BITS  bit;
};

struct HWBP_CNTL_BITS {                 
    Uint16 rsvd1:1;                     
    Uint16 BUS_SEL:4;                   
    Uint16 STOP:1;                      
    Uint16 RTOSINT:1;                   
    Uint16 COMP_MODE:3;                 
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:4;                     
};

union HWBP_CNTL_REG {
    Uint16  all;
    struct  HWBP_CNTL_BITS  bit;
};

struct HWBP_STATUS_BITS {               
    Uint16 EVENT_FIRED:1;               
    Uint16 rsvd1:7;                     
    Uint16 MODULE_ID:6;                 
    Uint16 STATUS:2;                    
};

union HWBP_STATUS_REG {
    Uint16  all;
    struct  HWBP_STATUS_BITS  bit;
};

struct ERAD_HWBP_REGS {
    Uint32                                   HWBP_MASK;                    
    Uint32                                   HWBP_REF;                     
    union   HWBP_CLEAR_REG                   HWBP_CLEAR;                   
    Uint16                                   rsvd1;                        
    union   HWBP_CNTL_REG                    HWBP_CNTL;                    
    union   HWBP_STATUS_REG                  HWBP_STATUS;                  
};

struct CTM_CNTL_BITS {                  
    Uint16 rsvd1:2;                     
    Uint16 START_STOP_MODE:1;           
    Uint16 EVENT_MODE:1;                
    Uint16 RST_ON_MATCH:1;              
    Uint16 rsvd2:1;                     
    Uint16 STOP:1;                      
    Uint16 RTOSINT:1;                   
    Uint16 START_STOP_CUMULATIVE:1;     
    Uint16 rsvd3:1;                     
    Uint16 RST_EN:1;                    
    Uint16 CNT_INP_SEL_EN:1;            
    Uint16 rsvd4:4;                     
};

union CTM_CNTL_REG {
    Uint16  all;
    struct  CTM_CNTL_BITS  bit;
};

struct CTM_STATUS_BITS {                
    Uint16 EVENT_FIRED:1;               
    Uint16 OVERFLOW:1;                  
    Uint16 MODULE_ID:10;                
    Uint16 STATUS:4;                    
};

union CTM_STATUS_REG {
    Uint16  all;
    struct  CTM_STATUS_BITS  bit;
};

struct CTM_INPUT_SEL_BITS {             
    Uint16 CNT_INP_SEL:7;               
    Uint16 rsvd1:1;                     
    Uint16 STA_INP_SEL:7;               
    Uint16 rsvd2:1;                     
};

union CTM_INPUT_SEL_REG {
    Uint16  all;
    struct  CTM_INPUT_SEL_BITS  bit;
};

struct CTM_CLEAR_BITS {                 
    Uint16 EVENT_CLEAR:1;               
    Uint16 OVERFLOW_CLEAR:1;            
    Uint16 rsvd1:14;                    
};

union CTM_CLEAR_REG {
    Uint16  all;
    struct  CTM_CLEAR_BITS  bit;
};

struct CTM_INPUT_SEL_2_BITS {           
    Uint16 STO_INP_SEL:7;               
    Uint16 rsvd1:1;                     
    Uint16 RST_INP_SEL:7;               
    Uint16 rsvd2:1;                     
};

union CTM_INPUT_SEL_2_REG {
    Uint16  all;
    struct  CTM_INPUT_SEL_2_BITS  bit;
};

struct CTM_INPUT_COND_BITS {            
    Uint16 CTM_INP_INV:1;               
    Uint16 CTM_INP_SYNCH:1;             
    Uint16 rsvd1:2;                     
    Uint16 STA_INP_INV:1;               
    Uint16 STA_INP_SYNCH:1;             
    Uint16 rsvd2:2;                     
    Uint16 STO_INP_INV:1;               
    Uint16 STO_INP_SYNCH:1;             
    Uint16 rsvd3:2;                     
    Uint16 RST_INP_INV:1;               
    Uint16 RST_INP_SYNCH:1;             
    Uint16 rsvd4:2;                     
};

union CTM_INPUT_COND_REG {
    Uint16  all;
    struct  CTM_INPUT_COND_BITS  bit;
};

struct ERAD_COUNTER_REGS {
    union   CTM_CNTL_REG                     CTM_CNTL;                     
    union   CTM_STATUS_REG                   CTM_STATUS;                   
    Uint32                                   CTM_REF;                      
    Uint32                                   CTM_COUNT;                    
    Uint32                                   CTM_MAX_COUNT;                
    union   CTM_INPUT_SEL_REG                CTM_INPUT_SEL;                
    union   CTM_CLEAR_REG                    CTM_CLEAR;                    
    union   CTM_INPUT_SEL_2_REG              CTM_INPUT_SEL_2;              
    union   CTM_INPUT_COND_REG               CTM_INPUT_COND;               
};

struct CRC_GLOBAL_CTRL_BITS {           
    Uint16 CRC1_INIT:1;                 
    Uint16 CRC2_INIT:1;                 
    Uint16 CRC3_INIT:1;                 
    Uint16 CRC4_INIT:1;                 
    Uint16 CRC5_INIT:1;                 
    Uint16 CRC6_INIT:1;                 
    Uint16 CRC7_INIT:1;                 
    Uint16 CRC8_INIT:1;                 
    Uint16 CRC1_EN:1;                   
    Uint16 CRC2_EN:1;                   
    Uint16 CRC3_EN:1;                   
    Uint16 CRC4_EN:1;                   
    Uint16 CRC5_EN:1;                   
    Uint16 CRC6_EN:1;                   
    Uint16 CRC7_EN:1;                   
    Uint16 CRC8_EN:1;                   
};

union CRC_GLOBAL_CTRL_REG {
    Uint16  all;
    struct  CRC_GLOBAL_CTRL_BITS  bit;
};

struct ERAD_CRC_GLOBAL_REGS {
    union   CRC_GLOBAL_CTRL_REG              CRC_GLOBAL_CTRL;              
};

struct CRC_QUALIFIER_BITS {             
    Uint16 CRC_QUALIFIER:5;             
    Uint16 rsvd1:11;                    
};

union CRC_QUALIFIER_REG {
    Uint16  all;
    struct  CRC_QUALIFIER_BITS  bit;
};

struct ERAD_CRC_REGS {
    Uint32                                   CRC_CURRENT;                  
    Uint32                                   CRC_SEED;                     
    union   CRC_QUALIFIER_REG                CRC_QUALIFIER;                
};




extern volatile struct ERAD_GLOBAL_REGS EradGlobalRegs;
extern volatile struct ERAD_HWBP_REGS EradHWBP1Regs;
extern volatile struct ERAD_HWBP_REGS EradHWBP2Regs;
extern volatile struct ERAD_HWBP_REGS EradHWBP3Regs;
extern volatile struct ERAD_HWBP_REGS EradHWBP4Regs;
extern volatile struct ERAD_HWBP_REGS EradHWBP5Regs;
extern volatile struct ERAD_HWBP_REGS EradHWBP6Regs;
extern volatile struct ERAD_HWBP_REGS EradHWBP7Regs;
extern volatile struct ERAD_HWBP_REGS EradHWBP8Regs;
extern volatile struct ERAD_COUNTER_REGS EradCounter1Regs;
extern volatile struct ERAD_COUNTER_REGS EradCounter2Regs;
extern volatile struct ERAD_COUNTER_REGS EradCounter3Regs;
extern volatile struct ERAD_COUNTER_REGS EradCounter4Regs;
extern volatile struct ERAD_CRC_GLOBAL_REGS EradCRCGlobalRegs;
extern volatile struct ERAD_CRC_REGS EradCRC1Regs;
extern volatile struct ERAD_CRC_REGS EradCRC2Regs;
extern volatile struct ERAD_CRC_REGS EradCRC3Regs;
extern volatile struct ERAD_CRC_REGS EradCRC4Regs;
extern volatile struct ERAD_CRC_REGS EradCRC5Regs;
extern volatile struct ERAD_CRC_REGS EradCRC6Regs;
extern volatile struct ERAD_CRC_REGS EradCRC7Regs;
extern volatile struct ERAD_CRC_REGS EradCRC8Regs;



















































struct FRDCNTL_BITS {                   
    Uint16 rsvd1:8;                     
    Uint16 RWAIT:4;                     
    Uint16 rsvd2:4;                     
    Uint16 rsvd3:16;                    
};

union FRDCNTL_REG {
    Uint32  all;
    struct  FRDCNTL_BITS  bit;
};

struct FBAC_BITS {                      
    Uint16 rsvd1:8;                     
    Uint16 BAGP:8;                      
    Uint16 rsvd2:16;                    
};

union FBAC_REG {
    Uint32  all;
    struct  FBAC_BITS  bit;
};

struct FBFALLBACK_BITS {                
    Uint16 BNKPWR0:2;                   
    Uint16 BNKPWR1:2;                   
    Uint16 BNKPWR2:2;                   
    Uint16 rsvd1:10;                    
    Uint16 rsvd2:16;                    
};

union FBFALLBACK_REG {
    Uint32  all;
    struct  FBFALLBACK_BITS  bit;
};

struct FBPRDY_BITS {                    
    Uint16 BANK0RDY:1;                  
    Uint16 BANK1RDY:1;                  
    Uint16 BANK2RDY:1;                  
    Uint16 rsvd1:12;                    
    Uint16 PUMPRDY:1;                   
    Uint16 rsvd2:16;                    
};

union FBPRDY_REG {
    Uint32  all;
    struct  FBPRDY_BITS  bit;
};

struct FPAC1_BITS {                     
    Uint16 PMPPWR:1;                    
    Uint16 rsvd1:15;                    
    Uint16 PSLEEP:12;                   
    Uint16 rsvd2:4;                     
};

union FPAC1_REG {
    Uint32  all;
    struct  FPAC1_BITS  bit;
};

struct FPAC2_BITS {                     
    Uint16 PAGP:16;                     
    Uint16 rsvd1:16;                    
};

union FPAC2_REG {
    Uint32  all;
    struct  FPAC2_BITS  bit;
};

struct FMSTAT_BITS {                    
    Uint16 rsvd1:1;                     
    Uint16 PSUSP:1;                     
    Uint16 ESUSP:1;                     
    Uint16 VOLTSTAT:1;                  
    Uint16 CSTAT:1;                     
    Uint16 INVDAT:1;                    
    Uint16 PGM:1;                       
    Uint16 ERS:1;                       
    Uint16 BUSY:1;                      
    Uint16 rsvd2:1;                     
    Uint16 EV:1;                        
    Uint16 rsvd3:1;                     
    Uint16 PGV:1;                       
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:1;                     
    Uint16 rsvd8:1;                     
    Uint16 rsvd9:14;                    
};

union FMSTAT_REG {
    Uint32  all;
    struct  FMSTAT_BITS  bit;
};

struct FRD_INTF_CTRL_BITS {             
    Uint16 PREFETCH_EN:1;               
    Uint16 DATA_CACHE_EN:1;             
    Uint16 rsvd1:14;                    
    Uint16 rsvd2:16;                    
};

union FRD_INTF_CTRL_REG {
    Uint32  all;
    struct  FRD_INTF_CTRL_BITS  bit;
};

struct FLASH_CTRL_REGS {
    union   FRDCNTL_REG                      FRDCNTL;                      
    Uint16                                   rsvd1[28];                    
    union   FBAC_REG                         FBAC;                         
    union   FBFALLBACK_REG                   FBFALLBACK;                   
    union   FBPRDY_REG                       FBPRDY;                       
    union   FPAC1_REG                        FPAC1;                        
    union   FPAC2_REG                        FPAC2;                        
    Uint16                                   rsvd2[2];                     
    union   FMSTAT_REG                       FMSTAT;                       
    Uint16                                   rsvd3[340];                   
    union   FRD_INTF_CTRL_REG                FRD_INTF_CTRL;                
};

struct ECC_ENABLE_BITS {                
    Uint16 ENABLE:4;                    
    Uint16 rsvd1:12;                    
    Uint16 rsvd2:16;                    
};

union ECC_ENABLE_REG {
    Uint32  all;
    struct  ECC_ENABLE_BITS  bit;
};

struct ERR_STATUS_BITS {                
    Uint16 FAIL_0_L:1;                  
    Uint16 FAIL_1_L:1;                  
    Uint16 UNC_ERR_L:1;                 
    Uint16 rsvd1:13;                    
    Uint16 FAIL_0_H:1;                  
    Uint16 FAIL_1_H:1;                  
    Uint16 UNC_ERR_H:1;                 
    Uint16 rsvd2:13;                    
};

union ERR_STATUS_REG {
    Uint32  all;
    struct  ERR_STATUS_BITS  bit;
};

struct ERR_POS_BITS {                   
    Uint16 ERR_POS_L:6;                 
    Uint16 rsvd1:2;                     
    Uint16 ERR_TYPE_L:1;                
    Uint16 rsvd2:7;                     
    Uint16 ERR_POS_H:6;                 
    Uint16 rsvd3:2;                     
    Uint16 ERR_TYPE_H:1;                
    Uint16 rsvd4:7;                     
};

union ERR_POS_REG {
    Uint32  all;
    struct  ERR_POS_BITS  bit;
};

struct ERR_STATUS_CLR_BITS {            
    Uint16 FAIL_0_L_CLR:1;              
    Uint16 FAIL_1_L_CLR:1;              
    Uint16 UNC_ERR_L_CLR:1;             
    Uint16 rsvd1:13;                    
    Uint16 FAIL_0_H_CLR:1;              
    Uint16 FAIL_1_H_CLR:1;              
    Uint16 UNC_ERR_H_CLR:1;             
    Uint16 rsvd2:13;                    
};

union ERR_STATUS_CLR_REG {
    Uint32  all;
    struct  ERR_STATUS_CLR_BITS  bit;
};

struct ERR_CNT_BITS {                   
    Uint16 ERR_CNT:16;                  
    Uint16 rsvd1:16;                    
};

union ERR_CNT_REG {
    Uint32  all;
    struct  ERR_CNT_BITS  bit;
};

struct ERR_THRESHOLD_BITS {             
    Uint16 ERR_THRESHOLD:16;            
    Uint16 rsvd1:16;                    
};

union ERR_THRESHOLD_REG {
    Uint32  all;
    struct  ERR_THRESHOLD_BITS  bit;
};

struct ERR_INTFLG_BITS {                
    Uint16 SINGLE_ERR_INTFLG:1;         
    Uint16 UNC_ERR_INTFLG:1;            
    Uint16 rsvd1:14;                    
    Uint16 rsvd2:16;                    
};

union ERR_INTFLG_REG {
    Uint32  all;
    struct  ERR_INTFLG_BITS  bit;
};

struct ERR_INTCLR_BITS {                
    Uint16 SINGLE_ERR_INTCLR:1;         
    Uint16 UNC_ERR_INTCLR:1;            
    Uint16 rsvd1:14;                    
    Uint16 rsvd2:16;                    
};

union ERR_INTCLR_REG {
    Uint32  all;
    struct  ERR_INTCLR_BITS  bit;
};

struct FADDR_TEST_BITS {                
    Uint16 rsvd1:3;                     
    Uint16 ADDRL:13;                    
    Uint16 ADDRH:6;                     
    Uint16 rsvd2:10;                    
};

union FADDR_TEST_REG {
    Uint32  all;
    struct  FADDR_TEST_BITS  bit;
};

struct FECC_TEST_BITS {                 
    Uint16 ECC:8;                       
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union FECC_TEST_REG {
    Uint32  all;
    struct  FECC_TEST_BITS  bit;
};

struct FECC_CTRL_BITS {                 
    Uint16 ECC_TEST_EN:1;               
    Uint16 ECC_SELECT:1;                
    Uint16 DO_ECC_CALC:1;               
    Uint16 rsvd1:13;                    
    Uint16 rsvd2:16;                    
};

union FECC_CTRL_REG {
    Uint32  all;
    struct  FECC_CTRL_BITS  bit;
};

struct FECC_STATUS_BITS {               
    Uint16 SINGLE_ERR:1;                
    Uint16 UNC_ERR:1;                   
    Uint16 DATA_ERR_POS:6;              
    Uint16 ERR_TYPE:1;                  
    Uint16 rsvd1:7;                     
    Uint16 rsvd2:16;                    
};

union FECC_STATUS_REG {
    Uint32  all;
    struct  FECC_STATUS_BITS  bit;
};

struct FLASH_ECC_REGS {
    union   ECC_ENABLE_REG                   ECC_ENABLE;                   
    Uint32                                   SINGLE_ERR_ADDR_LOW;          
    Uint32                                   SINGLE_ERR_ADDR_HIGH;         
    Uint32                                   UNC_ERR_ADDR_LOW;             
    Uint32                                   UNC_ERR_ADDR_HIGH;            
    union   ERR_STATUS_REG                   ERR_STATUS;                   
    union   ERR_POS_REG                      ERR_POS;                      
    union   ERR_STATUS_CLR_REG               ERR_STATUS_CLR;               
    union   ERR_CNT_REG                      ERR_CNT;                      
    union   ERR_THRESHOLD_REG                ERR_THRESHOLD;                
    union   ERR_INTFLG_REG                   ERR_INTFLG;                   
    union   ERR_INTCLR_REG                   ERR_INTCLR;                   
    Uint32                                   FDATAH_TEST;                  
    Uint32                                   FDATAL_TEST;                  
    union   FADDR_TEST_REG                   FADDR_TEST;                   
    union   FECC_TEST_REG                    FECC_TEST;                    
    union   FECC_CTRL_REG                    FECC_CTRL;                    
    Uint32                                   FOUTH_TEST;                   
    Uint32                                   FOUTL_TEST;                   
    union   FECC_STATUS_REG                  FECC_STATUS;                  
};




extern volatile struct FLASH_CTRL_REGS Flash0CtrlRegs;
extern volatile struct FLASH_ECC_REGS Flash0EccRegs;



















































struct TX_MASTER_CTRL_BITS {            
    Uint16 CORE_RST:1;                  
    Uint16 FLUSH:1;                     
    Uint16 rsvd1:6;                     
    Uint16 KEY:8;                       
};

union TX_MASTER_CTRL_REG {
    Uint16  all;
    struct  TX_MASTER_CTRL_BITS  bit;
};

struct TX_CLK_CTRL_BITS {               
    Uint16 CLK_RST:1;                   
    Uint16 CLK_EN:1;                    
    Uint16 PRESCALE_VAL:8;              
    Uint16 rsvd1:6;                     
};

union TX_CLK_CTRL_REG {
    Uint16  all;
    struct  TX_CLK_CTRL_BITS  bit;
};

struct TX_OPER_CTRL_LO_BITS {           
    Uint16 DATA_WIDTH:2;                
    Uint16 SPI_MODE:1;                  
    Uint16 START_MODE:3;                
    Uint16 SW_CRC:1;                    
    Uint16 PING_TO_MODE:1;              
    Uint16 SEL_PLLCLK:1;                
    Uint16 TDM_ENABLE:1;                
    Uint16 SEL_TDM_IN:1;                
    Uint16 rsvd1:5;                     
};

union TX_OPER_CTRL_LO_REG {
    Uint16  all;
    struct  TX_OPER_CTRL_LO_BITS  bit;
};

struct TX_OPER_CTRL_HI_BITS {           
    Uint16 rsvd1:5;                     
    Uint16 FORCE_ERR:1;                 
    Uint16 ECC_SEL:1;                   
    Uint16 EXT_TRIG_SEL:6;              
    Uint16 rsvd2:3;                     
};

union TX_OPER_CTRL_HI_REG {
    Uint16  all;
    struct  TX_OPER_CTRL_HI_BITS  bit;
};

struct TX_FRAME_CTRL_BITS {             
    Uint16 FRAME_TYPE:4;                
    Uint16 N_WORDS:4;                   
    Uint16 rsvd1:7;                     
    Uint16 START:1;                     
};

union TX_FRAME_CTRL_REG {
    Uint16  all;
    struct  TX_FRAME_CTRL_BITS  bit;
};

struct TX_FRAME_TAG_UDATA_BITS {        
    Uint16 FRAME_TAG:4;                 
    Uint16 rsvd1:4;                     
    Uint16 USER_DATA:8;                 
};

union TX_FRAME_TAG_UDATA_REG {
    Uint16  all;
    struct  TX_FRAME_TAG_UDATA_BITS  bit;
};

struct TX_BUF_PTR_LOAD_BITS {           
    Uint16 BUF_PTR_LOAD:4;              
    Uint16 rsvd1:12;                    
};

union TX_BUF_PTR_LOAD_REG {
    Uint16  all;
    struct  TX_BUF_PTR_LOAD_BITS  bit;
};

struct TX_BUF_PTR_STS_BITS {            
    Uint16 CURR_BUF_PTR:4;              
    Uint16 rsvd1:4;                     
    Uint16 CURR_WORD_CNT:5;             
    Uint16 rsvd2:3;                     
};

union TX_BUF_PTR_STS_REG {
    Uint16  all;
    struct  TX_BUF_PTR_STS_BITS  bit;
};

struct TX_PING_CTRL_BITS {              
    Uint16 CNT_RST:1;                   
    Uint16 TIMER_EN:1;                  
    Uint16 EXT_TRIG_EN:1;               
    Uint16 EXT_TRIG_SEL:6;              
    Uint16 rsvd1:7;                     
};

union TX_PING_CTRL_REG {
    Uint16  all;
    struct  TX_PING_CTRL_BITS  bit;
};

struct TX_PING_TAG_BITS {               
    Uint16 TAG:4;                       
    Uint16 rsvd1:12;                    
};

union TX_PING_TAG_REG {
    Uint16  all;
    struct  TX_PING_TAG_BITS  bit;
};

struct TX_INT_CTRL_BITS {               
    Uint16 INT1_EN_FRAME_DONE:1;        
    Uint16 INT1_EN_BUF_UNDERRUN:1;      
    Uint16 INT1_EN_BUF_OVERRUN:1;       
    Uint16 INT1_EN_PING_TO:1;           
    Uint16 rsvd1:4;                     
    Uint16 INT2_EN_FRAME_DONE:1;        
    Uint16 INT2_EN_BUF_UNDERRUN:1;      
    Uint16 INT2_EN_BUF_OVERRUN:1;       
    Uint16 INT2_EN_PING_TO:1;           
    Uint16 rsvd2:4;                     
};

union TX_INT_CTRL_REG {
    Uint16  all;
    struct  TX_INT_CTRL_BITS  bit;
};

struct TX_DMA_CTRL_BITS {               
    Uint16 DMA_EVT_EN:1;                
    Uint16 rsvd1:15;                    
};

union TX_DMA_CTRL_REG {
    Uint16  all;
    struct  TX_DMA_CTRL_BITS  bit;
};

struct TX_LOCK_CTRL_BITS {              
    Uint16 LOCK:1;                      
    Uint16 rsvd1:7;                     
    Uint16 KEY:8;                       
};

union TX_LOCK_CTRL_REG {
    Uint16  all;
    struct  TX_LOCK_CTRL_BITS  bit;
};

struct TX_EVT_STS_BITS {                
    Uint16 FRAME_DONE:1;                
    Uint16 BUF_UNDERRUN:1;              
    Uint16 BUF_OVERRUN:1;               
    Uint16 PING_TRIGGERED:1;            
    Uint16 rsvd1:12;                    
};

union TX_EVT_STS_REG {
    Uint16  all;
    struct  TX_EVT_STS_BITS  bit;
};

struct TX_EVT_CLR_BITS {                
    Uint16 FRAME_DONE:1;                
    Uint16 BUF_UNDERRUN:1;              
    Uint16 BUF_OVERRUN:1;               
    Uint16 PING_TRIGGERED:1;            
    Uint16 rsvd1:12;                    
};

union TX_EVT_CLR_REG {
    Uint16  all;
    struct  TX_EVT_CLR_BITS  bit;
};

struct TX_EVT_FRC_BITS {                
    Uint16 FRAME_DONE:1;                
    Uint16 BUF_UNDERRUN:1;              
    Uint16 BUF_OVERRUN:1;               
    Uint16 PING_TRIGGERED:1;            
    Uint16 rsvd1:12;                    
};

union TX_EVT_FRC_REG {
    Uint16  all;
    struct  TX_EVT_FRC_BITS  bit;
};

struct TX_USER_CRC_BITS {               
    Uint16 USER_CRC:8;                  
    Uint16 rsvd1:8;                     
};

union TX_USER_CRC_REG {
    Uint16  all;
    struct  TX_USER_CRC_BITS  bit;
};

struct TX_ECC_DATA_BITS {               
    Uint16 DATA_LOW:16;                 
    Uint16 DATA_HIGH:16;                
};

union TX_ECC_DATA_REG {
    Uint32  all;
    struct  TX_ECC_DATA_BITS  bit;
};

struct TX_ECC_VAL_BITS {                
    Uint16 ECC_VAL:7;                   
    Uint16 rsvd1:9;                     
};

union TX_ECC_VAL_REG {
    Uint16  all;
    struct  TX_ECC_VAL_BITS  bit;
};

struct TX_DLYLINE_CTRL_BITS {           
    Uint16 TXCLK_DLY:5;                 
    Uint16 TXD0_DLY:5;                  
    Uint16 TXD1_DLY:5;                  
    Uint16 rsvd1:1;                     
};

union TX_DLYLINE_CTRL_REG {
    Uint16  all;
    struct  TX_DLYLINE_CTRL_BITS  bit;
};

struct FSI_TX_REGS {
    union   TX_MASTER_CTRL_REG               TX_MASTER_CTRL;               
    Uint16                                   rsvd1;                        
    union   TX_CLK_CTRL_REG                  TX_CLK_CTRL;                  
    Uint16                                   rsvd2;                        
    union   TX_OPER_CTRL_LO_REG              TX_OPER_CTRL_LO;              
    union   TX_OPER_CTRL_HI_REG              TX_OPER_CTRL_HI;              
    union   TX_FRAME_CTRL_REG                TX_FRAME_CTRL;                
    union   TX_FRAME_TAG_UDATA_REG           TX_FRAME_TAG_UDATA;           
    union   TX_BUF_PTR_LOAD_REG              TX_BUF_PTR_LOAD;              
    union   TX_BUF_PTR_STS_REG               TX_BUF_PTR_STS;               
    union   TX_PING_CTRL_REG                 TX_PING_CTRL;                 
    union   TX_PING_TAG_REG                  TX_PING_TAG;                  
    Uint32                                   TX_PING_TO_REF;               
    Uint32                                   TX_PING_TO_CNT;               
    union   TX_INT_CTRL_REG                  TX_INT_CTRL;                  
    union   TX_DMA_CTRL_REG                  TX_DMA_CTRL;                  
    union   TX_LOCK_CTRL_REG                 TX_LOCK_CTRL;                 
    Uint16                                   rsvd3;                        
    union   TX_EVT_STS_REG                   TX_EVT_STS;                   
    Uint16                                   rsvd4;                        
    union   TX_EVT_CLR_REG                   TX_EVT_CLR;                   
    union   TX_EVT_FRC_REG                   TX_EVT_FRC;                   
    union   TX_USER_CRC_REG                  TX_USER_CRC;                  
    Uint16                                   rsvd5[7];                     
    union   TX_ECC_DATA_REG                  TX_ECC_DATA;                  
    union   TX_ECC_VAL_REG                   TX_ECC_VAL;                   
    Uint16                                   rsvd6;                        
    union   TX_DLYLINE_CTRL_REG              TX_DLYLINE_CTRL;              
    Uint16                                   rsvd7[27];                    
    Uint16                                   TX_BUF_BASE[16];              
};

struct RX_MASTER_CTRL_BITS {            
    Uint16 CORE_RST:1;                  
    Uint16 INT_LOOPBACK:1;              
    Uint16 SPI_PAIRING:1;               
    Uint16 INPUT_ISOLATE:1;             
    Uint16 DATA_FILTER_EN:1;            
    Uint16 rsvd1:3;                     
    Uint16 KEY:8;                       
};

union RX_MASTER_CTRL_REG {
    Uint16  all;
    struct  RX_MASTER_CTRL_BITS  bit;
};

struct RX_OPER_CTRL_BITS {              
    Uint16 DATA_WIDTH:2;                
    Uint16 SPI_MODE:1;                  
    Uint16 N_WORDS:4;                   
    Uint16 ECC_SEL:1;                   
    Uint16 PING_WD_RST_MODE:1;          
    Uint16 rsvd1:7;                     
};

union RX_OPER_CTRL_REG {
    Uint16  all;
    struct  RX_OPER_CTRL_BITS  bit;
};

struct RX_FRAME_INFO_BITS {             
    Uint16 FRAME_TYPE:4;                
    Uint16 rsvd1:12;                    
};

union RX_FRAME_INFO_REG {
    Uint16  all;
    struct  RX_FRAME_INFO_BITS  bit;
};

struct RX_FRAME_TAG_UDATA_BITS {        
    Uint16 rsvd1:1;                     
    Uint16 FRAME_TAG:4;                 
    Uint16 rsvd2:3;                     
    Uint16 USER_DATA:8;                 
};

union RX_FRAME_TAG_UDATA_REG {
    Uint16  all;
    struct  RX_FRAME_TAG_UDATA_BITS  bit;
};

struct RX_DMA_CTRL_BITS {               
    Uint16 DMA_EVT_EN:1;                
    Uint16 rsvd1:15;                    
};

union RX_DMA_CTRL_REG {
    Uint16  all;
    struct  RX_DMA_CTRL_BITS  bit;
};

struct RX_EVT_STS_BITS {                
    Uint16 PING_WD_TO:1;                
    Uint16 FRAME_WD_TO:1;               
    Uint16 CRC_ERR:1;                   
    Uint16 TYPE_ERR:1;                  
    Uint16 EOF_ERR:1;                   
    Uint16 BUF_OVERRUN:1;               
    Uint16 FRAME_DONE:1;                
    Uint16 BUF_UNDERRUN:1;              
    Uint16 ERR_FRAME:1;                 
    Uint16 PING_FRAME:1;                
    Uint16 FRAME_OVERRUN:1;             
    Uint16 DATA_FRAME:1;                
    Uint16 PING_TAG_MATCH:1;            
    Uint16 DATA_TAG_MATCH:1;            
    Uint16 ERROR_TAG_MATCH:1;           
    Uint16 rsvd1:1;                     
};

union RX_EVT_STS_REG {
    Uint16  all;
    struct  RX_EVT_STS_BITS  bit;
};

struct RX_CRC_INFO_BITS {               
    Uint16 RX_CRC:8;                    
    Uint16 CALC_CRC:8;                  
};

union RX_CRC_INFO_REG {
    Uint16  all;
    struct  RX_CRC_INFO_BITS  bit;
};

struct RX_EVT_CLR_BITS {                
    Uint16 PING_WD_TO:1;                
    Uint16 FRAME_WD_TO:1;               
    Uint16 CRC_ERR:1;                   
    Uint16 TYPE_ERR:1;                  
    Uint16 EOF_ERR:1;                   
    Uint16 BUF_OVERRUN:1;               
    Uint16 FRAME_DONE:1;                
    Uint16 BUF_UNDERRUN:1;              
    Uint16 ERR_FRAME:1;                 
    Uint16 PING_FRAME:1;                
    Uint16 FRAME_OVERRUN:1;             
    Uint16 DATA_FRAME:1;                
    Uint16 PING_TAG_MATCH:1;            
    Uint16 DATA_TAG_MATCH:1;            
    Uint16 ERROR_TAG_MATCH:1;           
    Uint16 rsvd1:1;                     
};

union RX_EVT_CLR_REG {
    Uint16  all;
    struct  RX_EVT_CLR_BITS  bit;
};

struct RX_EVT_FRC_BITS {                
    Uint16 PING_WD_TO:1;                
    Uint16 FRAME_WD_TO:1;               
    Uint16 CRC_ERR:1;                   
    Uint16 TYPE_ERR:1;                  
    Uint16 EOF_ERR:1;                   
    Uint16 BUF_OVERRUN:1;               
    Uint16 FRAME_DONE:1;                
    Uint16 BUF_UNDERRUN:1;              
    Uint16 ERR_FRAME:1;                 
    Uint16 PING_FRAME:1;                
    Uint16 FRAME_OVERRUN:1;             
    Uint16 DATA_FRAME:1;                
    Uint16 PING_TAG_MATCH:1;            
    Uint16 DATA_TAG_MATCH:1;            
    Uint16 ERROR_TAG_MATCH:1;           
    Uint16 rsvd1:1;                     
};

union RX_EVT_FRC_REG {
    Uint16  all;
    struct  RX_EVT_FRC_BITS  bit;
};

struct RX_BUF_PTR_LOAD_BITS {           
    Uint16 BUF_PTR_LOAD:4;              
    Uint16 rsvd1:12;                    
};

union RX_BUF_PTR_LOAD_REG {
    Uint16  all;
    struct  RX_BUF_PTR_LOAD_BITS  bit;
};

struct RX_BUF_PTR_STS_BITS {            
    Uint16 CURR_BUF_PTR:4;              
    Uint16 rsvd1:4;                     
    Uint16 CURR_WORD_CNT:5;             
    Uint16 rsvd2:3;                     
};

union RX_BUF_PTR_STS_REG {
    Uint16  all;
    struct  RX_BUF_PTR_STS_BITS  bit;
};

struct RX_FRAME_WD_CTRL_BITS {          
    Uint16 FRAME_WD_CNT_RST:1;          
    Uint16 FRAME_WD_EN:1;               
    Uint16 rsvd1:14;                    
};

union RX_FRAME_WD_CTRL_REG {
    Uint16  all;
    struct  RX_FRAME_WD_CTRL_BITS  bit;
};

struct RX_PING_WD_CTRL_BITS {           
    Uint16 PING_WD_RST:1;               
    Uint16 PING_WD_EN:1;                
    Uint16 rsvd1:14;                    
};

union RX_PING_WD_CTRL_REG {
    Uint16  all;
    struct  RX_PING_WD_CTRL_BITS  bit;
};

struct RX_PING_TAG_BITS {               
    Uint16 rsvd1:1;                     
    Uint16 PING_TAG:4;                  
    Uint16 rsvd2:11;                    
};

union RX_PING_TAG_REG {
    Uint16  all;
    struct  RX_PING_TAG_BITS  bit;
};

struct RX_INT1_CTRL_BITS {              
    Uint16 INT1_EN_PING_WD_TO:1;        
    Uint16 INT1_EN_FRAME_WD_TO:1;       
    Uint16 INT1_EN_CRC_ERR:1;           
    Uint16 INT1_EN_TYPE_ERR:1;          
    Uint16 INT1_EN_EOF_ERR:1;           
    Uint16 INT1_EN_OVERRUN:1;           
    Uint16 INT1_EN_FRAME_DONE:1;        
    Uint16 INT1_EN_UNDERRUN:1;          
    Uint16 INT1_EN_ERR_FRAME:1;         
    Uint16 INT1_EN_PING_FRAME:1;        
    Uint16 INT1_EN_FRAME_OVERRUN:1;     
    Uint16 INT1_EN_DATA_FRAME:1;        
    Uint16 INT1_EN_PING_TAG_MATCH:1;    
    Uint16 INT1_EN_DATA_TAG_MATCH:1;    
    Uint16 INT1_EN_ERROR_TAG_MATCH:1;   
    Uint16 rsvd1:1;                     
};

union RX_INT1_CTRL_REG {
    Uint16  all;
    struct  RX_INT1_CTRL_BITS  bit;
};

struct RX_INT2_CTRL_BITS {              
    Uint16 INT2_EN_PING_WD_TO:1;        
    Uint16 INT2_EN_FRAME_WD_TO:1;       
    Uint16 INT2_EN_CRC_ERR:1;           
    Uint16 INT2_EN_TYPE_ERR:1;          
    Uint16 INT2_EN_EOF_ERR:1;           
    Uint16 INT2_EN_OVERRUN:1;           
    Uint16 INT2_EN_FRAME_DONE:1;        
    Uint16 INT2_EN_UNDERRUN:1;          
    Uint16 INT2_EN_ERR_FRAME:1;         
    Uint16 INT2_EN_PING_FRAME:1;        
    Uint16 INT2_EN_FRAME_OVERRUN:1;     
    Uint16 INT2_EN_DATA_FRAME:1;        
    Uint16 INT2_EN_PING_TAG_MATCH:1;    
    Uint16 INT2_EN_DATA_TAG_MATCH:1;    
    Uint16 INT2_EN_ERROR_TAG_MATCH:1;   
    Uint16 rsvd1:1;                     
};

union RX_INT2_CTRL_REG {
    Uint16  all;
    struct  RX_INT2_CTRL_BITS  bit;
};

struct RX_LOCK_CTRL_BITS {              
    Uint16 LOCK:1;                      
    Uint16 rsvd1:7;                     
    Uint16 KEY:8;                       
};

union RX_LOCK_CTRL_REG {
    Uint16  all;
    struct  RX_LOCK_CTRL_BITS  bit;
};

struct RX_ECC_DATA_BITS {               
    Uint16 DATA_LOW:16;                 
    Uint16 DATA_HIGH:16;                
};

union RX_ECC_DATA_REG {
    Uint32  all;
    struct  RX_ECC_DATA_BITS  bit;
};

struct RX_ECC_VAL_BITS {                
    Uint16 ECC_VAL:7;                   
    Uint16 rsvd1:9;                     
};

union RX_ECC_VAL_REG {
    Uint16  all;
    struct  RX_ECC_VAL_BITS  bit;
};

struct RX_ECC_LOG_BITS {                
    Uint16 SBE:1;                       
    Uint16 MBE:1;                       
    Uint16 rsvd1:14;                    
};

union RX_ECC_LOG_REG {
    Uint16  all;
    struct  RX_ECC_LOG_BITS  bit;
};

struct RX_FRAME_TAG_CMP_BITS {          
    Uint16 TAG_REF:4;                   
    Uint16 TAG_MASK:4;                  
    Uint16 CMP_EN:1;                    
    Uint16 BROADCAST_EN:1;              
    Uint16 rsvd1:6;                     
};

union RX_FRAME_TAG_CMP_REG {
    Uint16  all;
    struct  RX_FRAME_TAG_CMP_BITS  bit;
};

struct RX_PING_TAG_CMP_BITS {           
    Uint16 TAG_REF:4;                   
    Uint16 TAG_MASK:4;                  
    Uint16 CMP_EN:1;                    
    Uint16 BROADCAST_EN:1;              
    Uint16 rsvd1:6;                     
};

union RX_PING_TAG_CMP_REG {
    Uint16  all;
    struct  RX_PING_TAG_CMP_BITS  bit;
};

struct RX_TRIG_CTRL_0_BITS {            
    Uint16 TRIG_EN:1;                   
    Uint16 TRIG_SEL:4;                  
    Uint16 rsvd1:3;                     
    Uint32 RX_TRIG_DLY:24;              
};

union RX_TRIG_CTRL_0_REG {
    Uint32  all;
    struct  RX_TRIG_CTRL_0_BITS  bit;
};

struct RX_TRIG_WIDTH_0_BITS {           
    Uint16 RX_TRIG_WIDTH:16;            
    Uint16 rsvd1:16;                    
};

union RX_TRIG_WIDTH_0_REG {
    Uint32  all;
    struct  RX_TRIG_WIDTH_0_BITS  bit;
};

struct RX_DLYLINE_CTRL_BITS {           
    Uint16 RXCLK_DLY:5;                 
    Uint16 RXD0_DLY:5;                  
    Uint16 RXD1_DLY:5;                  
    Uint16 rsvd1:1;                     
};

union RX_DLYLINE_CTRL_REG {
    Uint16  all;
    struct  RX_DLYLINE_CTRL_BITS  bit;
};

struct RX_TRIG_CTRL_1_BITS {            
    Uint16 TRIG_EN:1;                   
    Uint16 TRIG_SEL:4;                  
    Uint16 rsvd1:3;                     
    Uint32 RX_TRIG_DLY:24;              
};

union RX_TRIG_CTRL_1_REG {
    Uint32  all;
    struct  RX_TRIG_CTRL_1_BITS  bit;
};

struct RX_TRIG_CTRL_2_BITS {            
    Uint16 TRIG_EN:1;                   
    Uint16 TRIG_SEL:4;                  
    Uint16 rsvd1:3;                     
    Uint32 RX_TRIG_DLY:24;              
};

union RX_TRIG_CTRL_2_REG {
    Uint32  all;
    struct  RX_TRIG_CTRL_2_BITS  bit;
};

struct RX_TRIG_CTRL_3_BITS {            
    Uint16 TRIG_EN:1;                   
    Uint16 TRIG_SEL:4;                  
    Uint16 rsvd1:3;                     
    Uint32 RX_TRIG_DLY:24;              
};

union RX_TRIG_CTRL_3_REG {
    Uint32  all;
    struct  RX_TRIG_CTRL_3_BITS  bit;
};

struct RX_VIS_1_BITS {                  
    Uint16 rsvd1:3;                     
    Uint16 RX_CORE_STS:1;               
    Uint16 rsvd2:12;                    
    Uint16 rsvd3:16;                    
};

union RX_VIS_1_REG {
    Uint32  all;
    struct  RX_VIS_1_BITS  bit;
};

struct RX_UDATA_FILTER_BITS {           
    Uint16 UDATA_REF:8;                 
    Uint16 UDATA_MASK:8;                
};

union RX_UDATA_FILTER_REG {
    Uint16  all;
    struct  RX_UDATA_FILTER_BITS  bit;
};

struct FSI_RX_REGS {
    union   RX_MASTER_CTRL_REG               RX_MASTER_CTRL;               
    Uint16                                   rsvd1[3];                     
    union   RX_OPER_CTRL_REG                 RX_OPER_CTRL;                 
    Uint16                                   rsvd2;                        
    union   RX_FRAME_INFO_REG                RX_FRAME_INFO;                
    union   RX_FRAME_TAG_UDATA_REG           RX_FRAME_TAG_UDATA;           
    union   RX_DMA_CTRL_REG                  RX_DMA_CTRL;                  
    Uint16                                   rsvd3;                        
    union   RX_EVT_STS_REG                   RX_EVT_STS;                   
    union   RX_CRC_INFO_REG                  RX_CRC_INFO;                  
    union   RX_EVT_CLR_REG                   RX_EVT_CLR;                   
    union   RX_EVT_FRC_REG                   RX_EVT_FRC;                   
    union   RX_BUF_PTR_LOAD_REG              RX_BUF_PTR_LOAD;              
    union   RX_BUF_PTR_STS_REG               RX_BUF_PTR_STS;               
    union   RX_FRAME_WD_CTRL_REG             RX_FRAME_WD_CTRL;             
    Uint16                                   rsvd4;                        
    Uint32                                   RX_FRAME_WD_REF;              
    Uint32                                   RX_FRAME_WD_CNT;              
    union   RX_PING_WD_CTRL_REG              RX_PING_WD_CTRL;              
    union   RX_PING_TAG_REG                  RX_PING_TAG;                  
    Uint32                                   RX_PING_WD_REF;               
    Uint32                                   RX_PING_WD_CNT;               
    union   RX_INT1_CTRL_REG                 RX_INT1_CTRL;                 
    union   RX_INT2_CTRL_REG                 RX_INT2_CTRL;                 
    union   RX_LOCK_CTRL_REG                 RX_LOCK_CTRL;                 
    Uint16                                   rsvd5;                        
    union   RX_ECC_DATA_REG                  RX_ECC_DATA;                  
    union   RX_ECC_VAL_REG                   RX_ECC_VAL;                   
    Uint16                                   rsvd6;                        
    Uint32                                   RX_ECC_SEC_DATA;              
    union   RX_ECC_LOG_REG                   RX_ECC_LOG;                   
    Uint16                                   rsvd7;                        
    union   RX_FRAME_TAG_CMP_REG             RX_FRAME_TAG_CMP;             
    union   RX_PING_TAG_CMP_REG              RX_PING_TAG_CMP;              
    Uint16                                   rsvd8[2];                     
    union   RX_TRIG_CTRL_0_REG               RX_TRIG_CTRL_0;               
    union   RX_TRIG_WIDTH_0_REG              RX_TRIG_WIDTH_0;              
    union   RX_DLYLINE_CTRL_REG              RX_DLYLINE_CTRL;              
    Uint16                                   rsvd9;                        
    union   RX_TRIG_CTRL_1_REG               RX_TRIG_CTRL_1;               
    union   RX_TRIG_CTRL_2_REG               RX_TRIG_CTRL_2;               
    union   RX_TRIG_CTRL_3_REG               RX_TRIG_CTRL_3;               
    union   RX_VIS_1_REG                     RX_VIS_1;                     
    union   RX_UDATA_FILTER_REG              RX_UDATA_FILTER;              
    Uint16                                   rsvd10[5];                    
    Uint16                                   RX_BUF_BASE[16];              
};




extern volatile struct FSI_TX_REGS FsiTxaRegs;
extern volatile struct FSI_RX_REGS FsiRxaRegs;



















































struct GPACTRL_BITS {                   
    Uint16 QUALPRD0:8;                  
    Uint16 QUALPRD1:8;                  
    Uint16 QUALPRD2:8;                  
    Uint16 QUALPRD3:8;                  
};

union GPACTRL_REG {
    Uint32  all;
    struct  GPACTRL_BITS  bit;
};

struct GPAQSEL1_BITS {                  
    Uint16 GPIO0:2;                     
    Uint16 GPIO1:2;                     
    Uint16 GPIO2:2;                     
    Uint16 GPIO3:2;                     
    Uint16 GPIO4:2;                     
    Uint16 GPIO5:2;                     
    Uint16 GPIO6:2;                     
    Uint16 GPIO7:2;                     
    Uint16 GPIO8:2;                     
    Uint16 GPIO9:2;                     
    Uint16 GPIO10:2;                    
    Uint16 GPIO11:2;                    
    Uint16 GPIO12:2;                    
    Uint16 GPIO13:2;                    
    Uint16 GPIO14:2;                    
    Uint16 GPIO15:2;                    
};

union GPAQSEL1_REG {
    Uint32  all;
    struct  GPAQSEL1_BITS  bit;
};

struct GPAQSEL2_BITS {                  
    Uint16 GPIO16:2;                    
    Uint16 GPIO17:2;                    
    Uint16 GPIO18:2;                    
    Uint16 GPIO19:2;                    
    Uint16 GPIO20:2;                    
    Uint16 GPIO21:2;                    
    Uint16 GPIO22:2;                    
    Uint16 GPIO23:2;                    
    Uint16 GPIO24:2;                    
    Uint16 GPIO25:2;                    
    Uint16 GPIO26:2;                    
    Uint16 GPIO27:2;                    
    Uint16 GPIO28:2;                    
    Uint16 GPIO29:2;                    
    Uint16 GPIO30:2;                    
    Uint16 GPIO31:2;                    
};

union GPAQSEL2_REG {
    Uint32  all;
    struct  GPAQSEL2_BITS  bit;
};

struct GPAMUX1_BITS {                   
    Uint16 GPIO0:2;                     
    Uint16 GPIO1:2;                     
    Uint16 GPIO2:2;                     
    Uint16 GPIO3:2;                     
    Uint16 GPIO4:2;                     
    Uint16 GPIO5:2;                     
    Uint16 GPIO6:2;                     
    Uint16 GPIO7:2;                     
    Uint16 GPIO8:2;                     
    Uint16 GPIO9:2;                     
    Uint16 GPIO10:2;                    
    Uint16 GPIO11:2;                    
    Uint16 GPIO12:2;                    
    Uint16 GPIO13:2;                    
    Uint16 GPIO14:2;                    
    Uint16 GPIO15:2;                    
};

union GPAMUX1_REG {
    Uint32  all;
    struct  GPAMUX1_BITS  bit;
};

struct GPAMUX2_BITS {                   
    Uint16 GPIO16:2;                    
    Uint16 GPIO17:2;                    
    Uint16 GPIO18:2;                    
    Uint16 GPIO19:2;                    
    Uint16 GPIO20:2;                    
    Uint16 GPIO21:2;                    
    Uint16 GPIO22:2;                    
    Uint16 GPIO23:2;                    
    Uint16 GPIO24:2;                    
    Uint16 GPIO25:2;                    
    Uint16 GPIO26:2;                    
    Uint16 GPIO27:2;                    
    Uint16 GPIO28:2;                    
    Uint16 GPIO29:2;                    
    Uint16 GPIO30:2;                    
    Uint16 GPIO31:2;                    
};

union GPAMUX2_REG {
    Uint32  all;
    struct  GPAMUX2_BITS  bit;
};

struct GPADIR_BITS {                    
    Uint16 GPIO0:1;                     
    Uint16 GPIO1:1;                     
    Uint16 GPIO2:1;                     
    Uint16 GPIO3:1;                     
    Uint16 GPIO4:1;                     
    Uint16 GPIO5:1;                     
    Uint16 GPIO6:1;                     
    Uint16 GPIO7:1;                     
    Uint16 GPIO8:1;                     
    Uint16 GPIO9:1;                     
    Uint16 GPIO10:1;                    
    Uint16 GPIO11:1;                    
    Uint16 GPIO12:1;                    
    Uint16 GPIO13:1;                    
    Uint16 GPIO14:1;                    
    Uint16 GPIO15:1;                    
    Uint16 GPIO16:1;                    
    Uint16 GPIO17:1;                    
    Uint16 GPIO18:1;                    
    Uint16 GPIO19:1;                    
    Uint16 GPIO20:1;                    
    Uint16 GPIO21:1;                    
    Uint16 GPIO22:1;                    
    Uint16 GPIO23:1;                    
    Uint16 GPIO24:1;                    
    Uint16 GPIO25:1;                    
    Uint16 GPIO26:1;                    
    Uint16 GPIO27:1;                    
    Uint16 GPIO28:1;                    
    Uint16 GPIO29:1;                    
    Uint16 GPIO30:1;                    
    Uint16 GPIO31:1;                    
};

union GPADIR_REG {
    Uint32  all;
    struct  GPADIR_BITS  bit;
};

struct GPAPUD_BITS {                    
    Uint16 GPIO0:1;                     
    Uint16 GPIO1:1;                     
    Uint16 GPIO2:1;                     
    Uint16 GPIO3:1;                     
    Uint16 GPIO4:1;                     
    Uint16 GPIO5:1;                     
    Uint16 GPIO6:1;                     
    Uint16 GPIO7:1;                     
    Uint16 GPIO8:1;                     
    Uint16 GPIO9:1;                     
    Uint16 GPIO10:1;                    
    Uint16 GPIO11:1;                    
    Uint16 GPIO12:1;                    
    Uint16 GPIO13:1;                    
    Uint16 GPIO14:1;                    
    Uint16 GPIO15:1;                    
    Uint16 GPIO16:1;                    
    Uint16 GPIO17:1;                    
    Uint16 GPIO18:1;                    
    Uint16 GPIO19:1;                    
    Uint16 GPIO20:1;                    
    Uint16 GPIO21:1;                    
    Uint16 GPIO22:1;                    
    Uint16 GPIO23:1;                    
    Uint16 GPIO24:1;                    
    Uint16 GPIO25:1;                    
    Uint16 GPIO26:1;                    
    Uint16 GPIO27:1;                    
    Uint16 GPIO28:1;                    
    Uint16 GPIO29:1;                    
    Uint16 GPIO30:1;                    
    Uint16 GPIO31:1;                    
};

union GPAPUD_REG {
    Uint32  all;
    struct  GPAPUD_BITS  bit;
};

struct GPAINV_BITS {                    
    Uint16 GPIO0:1;                     
    Uint16 GPIO1:1;                     
    Uint16 GPIO2:1;                     
    Uint16 GPIO3:1;                     
    Uint16 GPIO4:1;                     
    Uint16 GPIO5:1;                     
    Uint16 GPIO6:1;                     
    Uint16 GPIO7:1;                     
    Uint16 GPIO8:1;                     
    Uint16 GPIO9:1;                     
    Uint16 GPIO10:1;                    
    Uint16 GPIO11:1;                    
    Uint16 GPIO12:1;                    
    Uint16 GPIO13:1;                    
    Uint16 GPIO14:1;                    
    Uint16 GPIO15:1;                    
    Uint16 GPIO16:1;                    
    Uint16 GPIO17:1;                    
    Uint16 GPIO18:1;                    
    Uint16 GPIO19:1;                    
    Uint16 GPIO20:1;                    
    Uint16 GPIO21:1;                    
    Uint16 GPIO22:1;                    
    Uint16 GPIO23:1;                    
    Uint16 GPIO24:1;                    
    Uint16 GPIO25:1;                    
    Uint16 GPIO26:1;                    
    Uint16 GPIO27:1;                    
    Uint16 GPIO28:1;                    
    Uint16 GPIO29:1;                    
    Uint16 GPIO30:1;                    
    Uint16 GPIO31:1;                    
};

union GPAINV_REG {
    Uint32  all;
    struct  GPAINV_BITS  bit;
};

struct GPAODR_BITS {                    
    Uint16 GPIO0:1;                     
    Uint16 GPIO1:1;                     
    Uint16 GPIO2:1;                     
    Uint16 GPIO3:1;                     
    Uint16 GPIO4:1;                     
    Uint16 GPIO5:1;                     
    Uint16 GPIO6:1;                     
    Uint16 GPIO7:1;                     
    Uint16 GPIO8:1;                     
    Uint16 GPIO9:1;                     
    Uint16 GPIO10:1;                    
    Uint16 GPIO11:1;                    
    Uint16 GPIO12:1;                    
    Uint16 GPIO13:1;                    
    Uint16 GPIO14:1;                    
    Uint16 GPIO15:1;                    
    Uint16 GPIO16:1;                    
    Uint16 GPIO17:1;                    
    Uint16 GPIO18:1;                    
    Uint16 GPIO19:1;                    
    Uint16 GPIO20:1;                    
    Uint16 GPIO21:1;                    
    Uint16 GPIO22:1;                    
    Uint16 GPIO23:1;                    
    Uint16 GPIO24:1;                    
    Uint16 GPIO25:1;                    
    Uint16 GPIO26:1;                    
    Uint16 GPIO27:1;                    
    Uint16 GPIO28:1;                    
    Uint16 GPIO29:1;                    
    Uint16 GPIO30:1;                    
    Uint16 GPIO31:1;                    
};

union GPAODR_REG {
    Uint32  all;
    struct  GPAODR_BITS  bit;
};

struct GPAAMSEL_BITS {                  
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:1;                     
    Uint16 rsvd8:1;                     
    Uint16 rsvd9:1;                     
    Uint16 rsvd10:1;                    
    Uint16 rsvd11:1;                    
    Uint16 rsvd12:1;                    
    Uint16 rsvd13:1;                    
    Uint16 rsvd14:1;                    
    Uint16 rsvd15:1;                    
    Uint16 rsvd16:1;                    
    Uint16 rsvd17:1;                    
    Uint16 rsvd18:1;                    
    Uint16 rsvd19:1;                    
    Uint16 rsvd20:1;                    
    Uint16 GPIO20:1;                    
    Uint16 GPIO21:1;                    
    Uint16 rsvd21:1;                    
    Uint16 rsvd22:1;                    
    Uint16 rsvd23:1;                    
    Uint16 rsvd24:1;                    
    Uint16 rsvd25:1;                    
    Uint16 rsvd26:1;                    
    Uint16 rsvd27:1;                    
    Uint16 rsvd28:1;                    
    Uint16 rsvd29:1;                    
    Uint16 rsvd30:1;                    
};

union GPAAMSEL_REG {
    Uint32  all;
    struct  GPAAMSEL_BITS  bit;
};

struct GPAGMUX1_BITS {                  
    Uint16 GPIO0:2;                     
    Uint16 GPIO1:2;                     
    Uint16 GPIO2:2;                     
    Uint16 GPIO3:2;                     
    Uint16 GPIO4:2;                     
    Uint16 GPIO5:2;                     
    Uint16 GPIO6:2;                     
    Uint16 GPIO7:2;                     
    Uint16 GPIO8:2;                     
    Uint16 GPIO9:2;                     
    Uint16 GPIO10:2;                    
    Uint16 GPIO11:2;                    
    Uint16 GPIO12:2;                    
    Uint16 GPIO13:2;                    
    Uint16 GPIO14:2;                    
    Uint16 GPIO15:2;                    
};

union GPAGMUX1_REG {
    Uint32  all;
    struct  GPAGMUX1_BITS  bit;
};

struct GPAGMUX2_BITS {                  
    Uint16 GPIO16:2;                    
    Uint16 GPIO17:2;                    
    Uint16 GPIO18:2;                    
    Uint16 GPIO19:2;                    
    Uint16 GPIO20:2;                    
    Uint16 GPIO21:2;                    
    Uint16 GPIO22:2;                    
    Uint16 GPIO23:2;                    
    Uint16 GPIO24:2;                    
    Uint16 GPIO25:2;                    
    Uint16 GPIO26:2;                    
    Uint16 GPIO27:2;                    
    Uint16 GPIO28:2;                    
    Uint16 GPIO29:2;                    
    Uint16 GPIO30:2;                    
    Uint16 GPIO31:2;                    
};

union GPAGMUX2_REG {
    Uint32  all;
    struct  GPAGMUX2_BITS  bit;
};

struct GPACSEL1_BITS {                  
    Uint16 GPIO0:4;                     
    Uint16 GPIO1:4;                     
    Uint16 GPIO2:4;                     
    Uint16 GPIO3:4;                     
    Uint16 GPIO4:4;                     
    Uint16 GPIO5:4;                     
    Uint16 GPIO6:4;                     
    Uint16 GPIO7:4;                     
};

union GPACSEL1_REG {
    Uint32  all;
    struct  GPACSEL1_BITS  bit;
};

struct GPACSEL2_BITS {                  
    Uint16 GPIO8:4;                     
    Uint16 GPIO9:4;                     
    Uint16 GPIO10:4;                    
    Uint16 GPIO11:4;                    
    Uint16 GPIO12:4;                    
    Uint16 GPIO13:4;                    
    Uint16 GPIO14:4;                    
    Uint16 GPIO15:4;                    
};

union GPACSEL2_REG {
    Uint32  all;
    struct  GPACSEL2_BITS  bit;
};

struct GPACSEL3_BITS {                  
    Uint16 GPIO16:4;                    
    Uint16 GPIO17:4;                    
    Uint16 GPIO18:4;                    
    Uint16 GPIO19:4;                    
    Uint16 GPIO20:4;                    
    Uint16 GPIO21:4;                    
    Uint16 GPIO22:4;                    
    Uint16 GPIO23:4;                    
};

union GPACSEL3_REG {
    Uint32  all;
    struct  GPACSEL3_BITS  bit;
};

struct GPACSEL4_BITS {                  
    Uint16 GPIO24:4;                    
    Uint16 GPIO25:4;                    
    Uint16 GPIO26:4;                    
    Uint16 GPIO27:4;                    
    Uint16 GPIO28:4;                    
    Uint16 GPIO29:4;                    
    Uint16 GPIO30:4;                    
    Uint16 GPIO31:4;                    
};

union GPACSEL4_REG {
    Uint32  all;
    struct  GPACSEL4_BITS  bit;
};

struct GPALOCK_BITS {                   
    Uint16 GPIO0:1;                     
    Uint16 GPIO1:1;                     
    Uint16 GPIO2:1;                     
    Uint16 GPIO3:1;                     
    Uint16 GPIO4:1;                     
    Uint16 GPIO5:1;                     
    Uint16 GPIO6:1;                     
    Uint16 GPIO7:1;                     
    Uint16 GPIO8:1;                     
    Uint16 GPIO9:1;                     
    Uint16 GPIO10:1;                    
    Uint16 GPIO11:1;                    
    Uint16 GPIO12:1;                    
    Uint16 GPIO13:1;                    
    Uint16 GPIO14:1;                    
    Uint16 GPIO15:1;                    
    Uint16 GPIO16:1;                    
    Uint16 GPIO17:1;                    
    Uint16 GPIO18:1;                    
    Uint16 GPIO19:1;                    
    Uint16 GPIO20:1;                    
    Uint16 GPIO21:1;                    
    Uint16 GPIO22:1;                    
    Uint16 GPIO23:1;                    
    Uint16 GPIO24:1;                    
    Uint16 GPIO25:1;                    
    Uint16 GPIO26:1;                    
    Uint16 GPIO27:1;                    
    Uint16 GPIO28:1;                    
    Uint16 GPIO29:1;                    
    Uint16 GPIO30:1;                    
    Uint16 GPIO31:1;                    
};

union GPALOCK_REG {
    Uint32  all;
    struct  GPALOCK_BITS  bit;
};

struct GPACR_BITS {                     
    Uint16 GPIO0:1;                     
    Uint16 GPIO1:1;                     
    Uint16 GPIO2:1;                     
    Uint16 GPIO3:1;                     
    Uint16 GPIO4:1;                     
    Uint16 GPIO5:1;                     
    Uint16 GPIO6:1;                     
    Uint16 GPIO7:1;                     
    Uint16 GPIO8:1;                     
    Uint16 GPIO9:1;                     
    Uint16 GPIO10:1;                    
    Uint16 GPIO11:1;                    
    Uint16 GPIO12:1;                    
    Uint16 GPIO13:1;                    
    Uint16 GPIO14:1;                    
    Uint16 GPIO15:1;                    
    Uint16 GPIO16:1;                    
    Uint16 GPIO17:1;                    
    Uint16 GPIO18:1;                    
    Uint16 GPIO19:1;                    
    Uint16 GPIO20:1;                    
    Uint16 GPIO21:1;                    
    Uint16 GPIO22:1;                    
    Uint16 GPIO23:1;                    
    Uint16 GPIO24:1;                    
    Uint16 GPIO25:1;                    
    Uint16 GPIO26:1;                    
    Uint16 GPIO27:1;                    
    Uint16 GPIO28:1;                    
    Uint16 GPIO29:1;                    
    Uint16 GPIO30:1;                    
    Uint16 GPIO31:1;                    
};

union GPACR_REG {
    Uint32  all;
    struct  GPACR_BITS  bit;
};

struct GPBCTRL_BITS {                   
    Uint16 QUALPRD0:8;                  
    Uint16 QUALPRD1:8;                  
    Uint16 QUALPRD2:8;                  
    Uint16 QUALPRD3:8;                  
};

union GPBCTRL_REG {
    Uint32  all;
    struct  GPBCTRL_BITS  bit;
};

struct GPBQSEL1_BITS {                  
    Uint16 GPIO32:2;                    
    Uint16 GPIO33:2;                    
    Uint16 GPIO34:2;                    
    Uint16 GPIO35:2;                    
    Uint16 rsvd1:2;                     
    Uint16 GPIO37:2;                    
    Uint16 rsvd2:2;                     
    Uint16 GPIO39:2;                    
    Uint16 GPIO40:2;                    
    Uint16 GPIO41:2;                    
    Uint16 GPIO42:2;                    
    Uint16 GPIO43:2;                    
    Uint16 GPIO44:2;                    
    Uint16 GPIO45:2;                    
    Uint16 GPIO46:2;                    
    Uint16 GPIO47:2;                    
};

union GPBQSEL1_REG {
    Uint32  all;
    struct  GPBQSEL1_BITS  bit;
};

struct GPBQSEL2_BITS {                  
    Uint16 GPIO48:2;                    
    Uint16 GPIO49:2;                    
    Uint16 GPIO50:2;                    
    Uint16 GPIO51:2;                    
    Uint16 GPIO52:2;                    
    Uint16 GPIO53:2;                    
    Uint16 GPIO54:2;                    
    Uint16 GPIO55:2;                    
    Uint16 GPIO56:2;                    
    Uint16 GPIO57:2;                    
    Uint16 GPIO58:2;                    
    Uint16 GPIO59:2;                    
    Uint16 GPIO60:2;                    
    Uint16 GPIO61:2;                    
    Uint16 rsvd1:2;                     
    Uint16 rsvd2:2;                     
};

union GPBQSEL2_REG {
    Uint32  all;
    struct  GPBQSEL2_BITS  bit;
};

struct GPBMUX1_BITS {                   
    Uint16 GPIO32:2;                    
    Uint16 GPIO33:2;                    
    Uint16 GPIO34:2;                    
    Uint16 GPIO35:2;                    
    Uint16 rsvd1:2;                     
    Uint16 GPIO37:2;                    
    Uint16 rsvd2:2;                     
    Uint16 GPIO39:2;                    
    Uint16 GPIO40:2;                    
    Uint16 GPIO41:2;                    
    Uint16 GPIO42:2;                    
    Uint16 GPIO43:2;                    
    Uint16 GPIO44:2;                    
    Uint16 GPIO45:2;                    
    Uint16 GPIO46:2;                    
    Uint16 GPIO47:2;                    
};

union GPBMUX1_REG {
    Uint32  all;
    struct  GPBMUX1_BITS  bit;
};

struct GPBMUX2_BITS {                   
    Uint16 GPIO48:2;                    
    Uint16 GPIO49:2;                    
    Uint16 GPIO50:2;                    
    Uint16 GPIO51:2;                    
    Uint16 GPIO52:2;                    
    Uint16 GPIO53:2;                    
    Uint16 GPIO54:2;                    
    Uint16 GPIO55:2;                    
    Uint16 GPIO56:2;                    
    Uint16 GPIO57:2;                    
    Uint16 GPIO58:2;                    
    Uint16 GPIO59:2;                    
    Uint16 GPIO60:2;                    
    Uint16 GPIO61:2;                    
    Uint16 rsvd1:2;                     
    Uint16 rsvd2:2;                     
};

union GPBMUX2_REG {
    Uint32  all;
    struct  GPBMUX2_BITS  bit;
};

struct GPBDIR_BITS {                    
    Uint16 GPIO32:1;                    
    Uint16 GPIO33:1;                    
    Uint16 GPIO34:1;                    
    Uint16 GPIO35:1;                    
    Uint16 rsvd1:1;                     
    Uint16 GPIO37:1;                    
    Uint16 rsvd2:1;                     
    Uint16 GPIO39:1;                    
    Uint16 GPIO40:1;                    
    Uint16 GPIO41:1;                    
    Uint16 GPIO42:1;                    
    Uint16 GPIO43:1;                    
    Uint16 GPIO44:1;                    
    Uint16 GPIO45:1;                    
    Uint16 GPIO46:1;                    
    Uint16 GPIO47:1;                    
    Uint16 GPIO48:1;                    
    Uint16 GPIO49:1;                    
    Uint16 GPIO50:1;                    
    Uint16 GPIO51:1;                    
    Uint16 GPIO52:1;                    
    Uint16 GPIO53:1;                    
    Uint16 GPIO54:1;                    
    Uint16 GPIO55:1;                    
    Uint16 GPIO56:1;                    
    Uint16 GPIO57:1;                    
    Uint16 GPIO58:1;                    
    Uint16 GPIO59:1;                    
    Uint16 GPIO60:1;                    
    Uint16 GPIO61:1;                    
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
};

union GPBDIR_REG {
    Uint32  all;
    struct  GPBDIR_BITS  bit;
};

struct GPBPUD_BITS {                    
    Uint16 GPIO32:1;                    
    Uint16 GPIO33:1;                    
    Uint16 GPIO34:1;                    
    Uint16 GPIO35:1;                    
    Uint16 rsvd1:1;                     
    Uint16 GPIO37:1;                    
    Uint16 rsvd2:1;                     
    Uint16 GPIO39:1;                    
    Uint16 GPIO40:1;                    
    Uint16 GPIO41:1;                    
    Uint16 GPIO42:1;                    
    Uint16 GPIO43:1;                    
    Uint16 GPIO44:1;                    
    Uint16 GPIO45:1;                    
    Uint16 GPIO46:1;                    
    Uint16 GPIO47:1;                    
    Uint16 GPIO48:1;                    
    Uint16 GPIO49:1;                    
    Uint16 GPIO50:1;                    
    Uint16 GPIO51:1;                    
    Uint16 GPIO52:1;                    
    Uint16 GPIO53:1;                    
    Uint16 GPIO54:1;                    
    Uint16 GPIO55:1;                    
    Uint16 GPIO56:1;                    
    Uint16 GPIO57:1;                    
    Uint16 GPIO58:1;                    
    Uint16 GPIO59:1;                    
    Uint16 GPIO60:1;                    
    Uint16 GPIO61:1;                    
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
};

union GPBPUD_REG {
    Uint32  all;
    struct  GPBPUD_BITS  bit;
};

struct GPBINV_BITS {                    
    Uint16 GPIO32:1;                    
    Uint16 GPIO33:1;                    
    Uint16 GPIO34:1;                    
    Uint16 GPIO35:1;                    
    Uint16 rsvd1:1;                     
    Uint16 GPIO37:1;                    
    Uint16 rsvd2:1;                     
    Uint16 GPIO39:1;                    
    Uint16 GPIO40:1;                    
    Uint16 GPIO41:1;                    
    Uint16 GPIO42:1;                    
    Uint16 GPIO43:1;                    
    Uint16 GPIO44:1;                    
    Uint16 GPIO45:1;                    
    Uint16 GPIO46:1;                    
    Uint16 GPIO47:1;                    
    Uint16 GPIO48:1;                    
    Uint16 GPIO49:1;                    
    Uint16 GPIO50:1;                    
    Uint16 GPIO51:1;                    
    Uint16 GPIO52:1;                    
    Uint16 GPIO53:1;                    
    Uint16 GPIO54:1;                    
    Uint16 GPIO55:1;                    
    Uint16 GPIO56:1;                    
    Uint16 GPIO57:1;                    
    Uint16 GPIO58:1;                    
    Uint16 GPIO59:1;                    
    Uint16 GPIO60:1;                    
    Uint16 GPIO61:1;                    
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
};

union GPBINV_REG {
    Uint32  all;
    struct  GPBINV_BITS  bit;
};

struct GPBODR_BITS {                    
    Uint16 GPIO32:1;                    
    Uint16 GPIO33:1;                    
    Uint16 GPIO34:1;                    
    Uint16 GPIO35:1;                    
    Uint16 rsvd1:1;                     
    Uint16 GPIO37:1;                    
    Uint16 rsvd2:1;                     
    Uint16 GPIO39:1;                    
    Uint16 GPIO40:1;                    
    Uint16 GPIO41:1;                    
    Uint16 GPIO42:1;                    
    Uint16 GPIO43:1;                    
    Uint16 GPIO44:1;                    
    Uint16 GPIO45:1;                    
    Uint16 GPIO46:1;                    
    Uint16 GPIO47:1;                    
    Uint16 GPIO48:1;                    
    Uint16 GPIO49:1;                    
    Uint16 GPIO50:1;                    
    Uint16 GPIO51:1;                    
    Uint16 GPIO52:1;                    
    Uint16 GPIO53:1;                    
    Uint16 GPIO54:1;                    
    Uint16 GPIO55:1;                    
    Uint16 GPIO56:1;                    
    Uint16 GPIO57:1;                    
    Uint16 GPIO58:1;                    
    Uint16 GPIO59:1;                    
    Uint16 GPIO60:1;                    
    Uint16 GPIO61:1;                    
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
};

union GPBODR_REG {
    Uint32  all;
    struct  GPBODR_BITS  bit;
};

struct GPBGMUX1_BITS {                  
    Uint16 GPIO32:2;                    
    Uint16 GPIO33:2;                    
    Uint16 GPIO34:2;                    
    Uint16 GPIO35:2;                    
    Uint16 rsvd1:2;                     
    Uint16 GPIO37:2;                    
    Uint16 rsvd2:2;                     
    Uint16 GPIO39:2;                    
    Uint16 GPIO40:2;                    
    Uint16 GPIO41:2;                    
    Uint16 GPIO42:2;                    
    Uint16 GPIO43:2;                    
    Uint16 GPIO44:2;                    
    Uint16 GPIO45:2;                    
    Uint16 GPIO46:2;                    
    Uint16 GPIO47:2;                    
};

union GPBGMUX1_REG {
    Uint32  all;
    struct  GPBGMUX1_BITS  bit;
};

struct GPBGMUX2_BITS {                  
    Uint16 GPIO48:2;                    
    Uint16 GPIO49:2;                    
    Uint16 GPIO50:2;                    
    Uint16 GPIO51:2;                    
    Uint16 GPIO52:2;                    
    Uint16 GPIO53:2;                    
    Uint16 GPIO54:2;                    
    Uint16 GPIO55:2;                    
    Uint16 GPIO56:2;                    
    Uint16 GPIO57:2;                    
    Uint16 GPIO58:2;                    
    Uint16 GPIO59:2;                    
    Uint16 GPIO60:2;                    
    Uint16 GPIO61:2;                    
    Uint16 rsvd1:2;                     
    Uint16 rsvd2:2;                     
};

union GPBGMUX2_REG {
    Uint32  all;
    struct  GPBGMUX2_BITS  bit;
};

struct GPBCSEL1_BITS {                  
    Uint16 GPIO32:4;                    
    Uint16 GPIO33:4;                    
    Uint16 GPIO34:4;                    
    Uint16 GPIO35:4;                    
    Uint16 rsvd1:4;                     
    Uint16 GPIO37:4;                    
    Uint16 rsvd2:4;                     
    Uint16 GPIO39:4;                    
};

union GPBCSEL1_REG {
    Uint32  all;
    struct  GPBCSEL1_BITS  bit;
};

struct GPBCSEL2_BITS {                  
    Uint16 GPIO40:4;                    
    Uint16 GPIO41:4;                    
    Uint16 GPIO42:4;                    
    Uint16 GPIO43:4;                    
    Uint16 GPIO44:4;                    
    Uint16 GPIO45:4;                    
    Uint16 GPIO46:4;                    
    Uint16 GPIO47:4;                    
};

union GPBCSEL2_REG {
    Uint32  all;
    struct  GPBCSEL2_BITS  bit;
};

struct GPBCSEL3_BITS {                  
    Uint16 GPIO48:4;                    
    Uint16 GPIO49:4;                    
    Uint16 GPIO50:4;                    
    Uint16 GPIO51:4;                    
    Uint16 GPIO52:4;                    
    Uint16 GPIO53:4;                    
    Uint16 GPIO54:4;                    
    Uint16 GPIO55:4;                    
};

union GPBCSEL3_REG {
    Uint32  all;
    struct  GPBCSEL3_BITS  bit;
};

struct GPBCSEL4_BITS {                  
    Uint16 GPIO56:4;                    
    Uint16 GPIO57:4;                    
    Uint16 GPIO58:4;                    
    Uint16 GPIO59:4;                    
    Uint16 GPIO60:4;                    
    Uint16 GPIO61:4;                    
    Uint16 rsvd1:4;                     
    Uint16 rsvd2:4;                     
};

union GPBCSEL4_REG {
    Uint32  all;
    struct  GPBCSEL4_BITS  bit;
};

struct GPBLOCK_BITS {                   
    Uint16 GPIO32:1;                    
    Uint16 GPIO33:1;                    
    Uint16 GPIO34:1;                    
    Uint16 GPIO35:1;                    
    Uint16 rsvd1:1;                     
    Uint16 GPIO37:1;                    
    Uint16 rsvd2:1;                     
    Uint16 GPIO39:1;                    
    Uint16 GPIO40:1;                    
    Uint16 GPIO41:1;                    
    Uint16 GPIO42:1;                    
    Uint16 GPIO43:1;                    
    Uint16 GPIO44:1;                    
    Uint16 GPIO45:1;                    
    Uint16 GPIO46:1;                    
    Uint16 GPIO47:1;                    
    Uint16 GPIO48:1;                    
    Uint16 GPIO49:1;                    
    Uint16 GPIO50:1;                    
    Uint16 GPIO51:1;                    
    Uint16 GPIO52:1;                    
    Uint16 GPIO53:1;                    
    Uint16 GPIO54:1;                    
    Uint16 GPIO55:1;                    
    Uint16 GPIO56:1;                    
    Uint16 GPIO57:1;                    
    Uint16 GPIO58:1;                    
    Uint16 GPIO59:1;                    
    Uint16 GPIO60:1;                    
    Uint16 GPIO61:1;                    
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
};

union GPBLOCK_REG {
    Uint32  all;
    struct  GPBLOCK_BITS  bit;
};

struct GPBCR_BITS {                     
    Uint16 GPIO32:1;                    
    Uint16 GPIO33:1;                    
    Uint16 GPIO34:1;                    
    Uint16 GPIO35:1;                    
    Uint16 rsvd1:1;                     
    Uint16 GPIO37:1;                    
    Uint16 rsvd2:1;                     
    Uint16 GPIO39:1;                    
    Uint16 GPIO40:1;                    
    Uint16 GPIO41:1;                    
    Uint16 GPIO42:1;                    
    Uint16 GPIO43:1;                    
    Uint16 GPIO44:1;                    
    Uint16 GPIO45:1;                    
    Uint16 GPIO46:1;                    
    Uint16 GPIO47:1;                    
    Uint16 GPIO48:1;                    
    Uint16 GPIO49:1;                    
    Uint16 GPIO50:1;                    
    Uint16 GPIO51:1;                    
    Uint16 GPIO52:1;                    
    Uint16 GPIO53:1;                    
    Uint16 GPIO54:1;                    
    Uint16 GPIO55:1;                    
    Uint16 GPIO56:1;                    
    Uint16 GPIO57:1;                    
    Uint16 GPIO58:1;                    
    Uint16 GPIO59:1;                    
    Uint16 GPIO60:1;                    
    Uint16 GPIO61:1;                    
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
};

union GPBCR_REG {
    Uint32  all;
    struct  GPBCR_BITS  bit;
};

struct GPHCTRL_BITS {                   
    Uint16 QUALPRD0:8;                  
    Uint16 QUALPRD1:8;                  
    Uint16 QUALPRD2:8;                  
    Uint16 QUALPRD3:8;                  
};

union GPHCTRL_REG {
    Uint32  all;
    struct  GPHCTRL_BITS  bit;
};

struct GPHQSEL1_BITS {                  
    Uint16 GPIO224:2;                   
    Uint16 GPIO225:2;                   
    Uint16 GPIO226:2;                   
    Uint16 GPIO227:2;                   
    Uint16 GPIO228:2;                   
    Uint16 GPIO229:2;                   
    Uint16 GPIO230:2;                   
    Uint16 GPIO231:2;                   
    Uint16 GPIO232:2;                   
    Uint16 GPIO233:2;                   
    Uint16 rsvd1:2;                     
    Uint16 rsvd2:2;                     
    Uint16 GPIO236:2;                   
    Uint16 GPIO237:2;                   
    Uint16 GPIO238:2;                   
    Uint16 GPIO239:2;                   
};

union GPHQSEL1_REG {
    Uint32  all;
    struct  GPHQSEL1_BITS  bit;
};

struct GPHQSEL2_BITS {                  
    Uint16 GPIO240:2;                   
    Uint16 GPIO241:2;                   
    Uint16 GPIO242:2;                   
    Uint16 rsvd1:2;                     
    Uint16 GPIO244:2;                   
    Uint16 GPIO245:2;                   
    Uint16 rsvd2:2;                     
    Uint16 GPIO247:2;                   
    Uint16 GPIO248:2;                   
    Uint16 GPIO249:2;                   
    Uint16 rsvd3:2;                     
    Uint16 GPIO251:2;                   
    Uint16 GPIO252:2;                   
    Uint16 GPIO253:2;                   
    Uint16 rsvd4:2;                     
    Uint16 rsvd5:2;                     
};

union GPHQSEL2_REG {
    Uint32  all;
    struct  GPHQSEL2_BITS  bit;
};

struct GPHMUX1_BITS {                   
    Uint16 GPIO224:2;                   
    Uint16 GPIO225:2;                   
    Uint16 GPIO226:2;                   
    Uint16 GPIO227:2;                   
    Uint16 GPIO228:2;                   
    Uint16 rsvd1:2;                     
    Uint16 GPIO230:2;                   
    Uint16 GPIO231:2;                   
    Uint16 GPIO232:2;                   
    Uint16 GPIO233:2;                   
    Uint16 rsvd2:2;                     
    Uint16 rsvd3:2;                     
    Uint16 rsvd4:2;                     
    Uint16 GPIO237:2;                   
    Uint16 GPIO238:2;                   
    Uint16 GPIO239:2;                   
};

union GPHMUX1_REG {
    Uint32  all;
    struct  GPHMUX1_BITS  bit;
};

struct GPHMUX2_BITS {                   
    Uint16 GPIO240:2;                   
    Uint16 GPIO241:2;                   
    Uint16 GPIO242:2;                   
    Uint16 rsvd1:2;                     
    Uint16 GPIO244:2;                   
    Uint16 GPIO245:2;                   
    Uint16 rsvd2:2;                     
    Uint16 rsvd3:2;                     
    Uint16 rsvd4:2;                     
    Uint16 rsvd5:2;                     
    Uint16 rsvd6:2;                     
    Uint16 rsvd7:2;                     
    Uint16 GPIO252:2;                   
    Uint16 rsvd8:2;                     
    Uint16 rsvd9:2;                     
    Uint16 rsvd10:2;                    
};

union GPHMUX2_REG {
    Uint32  all;
    struct  GPHMUX2_BITS  bit;
};

struct GPHINV_BITS {                    
    Uint16 GPIO224:1;                   
    Uint16 GPIO225:1;                   
    Uint16 GPIO226:1;                   
    Uint16 GPIO227:1;                   
    Uint16 GPIO228:1;                   
    Uint16 GPIO229:1;                   
    Uint16 GPIO230:1;                   
    Uint16 GPIO231:1;                   
    Uint16 GPIO232:1;                   
    Uint16 GPIO233:1;                   
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 GPIO236:1;                   
    Uint16 GPIO237:1;                   
    Uint16 GPIO238:1;                   
    Uint16 GPIO239:1;                   
    Uint16 GPIO240:1;                   
    Uint16 GPIO241:1;                   
    Uint16 GPIO242:1;                   
    Uint16 rsvd3:1;                     
    Uint16 GPIO244:1;                   
    Uint16 GPIO245:1;                   
    Uint16 rsvd4:1;                     
    Uint16 GPIO247:1;                   
    Uint16 GPIO248:1;                   
    Uint16 GPIO249:1;                   
    Uint16 rsvd5:1;                     
    Uint16 GPIO251:1;                   
    Uint16 GPIO252:1;                   
    Uint16 GPIO253:1;                   
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:1;                     
};

union GPHINV_REG {
    Uint32  all;
    struct  GPHINV_BITS  bit;
};

struct GPHAMSEL_BITS {                  
    Uint16 GPIO224:1;                   
    Uint16 GPIO225:1;                   
    Uint16 GPIO226:1;                   
    Uint16 GPIO227:1;                   
    Uint16 GPIO228:1;                   
    Uint16 GPIO229:1;                   
    Uint16 GPIO230:1;                   
    Uint16 GPIO231:1;                   
    Uint16 GPIO232:1;                   
    Uint16 GPIO233:1;                   
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 GPIO236:1;                   
    Uint16 GPIO237:1;                   
    Uint16 GPIO238:1;                   
    Uint16 GPIO239:1;                   
    Uint16 GPIO240:1;                   
    Uint16 GPIO241:1;                   
    Uint16 GPIO242:1;                   
    Uint16 rsvd3:1;                     
    Uint16 GPIO244:1;                   
    Uint16 GPIO245:1;                   
    Uint16 rsvd4:1;                     
    Uint16 GPIO247:1;                   
    Uint16 GPIO248:1;                   
    Uint16 GPIO249:1;                   
    Uint16 rsvd5:1;                     
    Uint16 GPIO251:1;                   
    Uint16 GPIO252:1;                   
    Uint16 GPIO253:1;                   
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:1;                     
};

union GPHAMSEL_REG {
    Uint32  all;
    struct  GPHAMSEL_BITS  bit;
};

struct GPHGMUX1_BITS {                  
    Uint16 GPIO224:2;                   
    Uint16 GPIO225:2;                   
    Uint16 GPIO226:2;                   
    Uint16 GPIO227:2;                   
    Uint16 GPIO228:2;                   
    Uint16 rsvd1:2;                     
    Uint16 GPIO230:2;                   
    Uint16 GPIO231:2;                   
    Uint16 GPIO232:2;                   
    Uint16 GPIO233:2;                   
    Uint16 rsvd2:2;                     
    Uint16 rsvd3:2;                     
    Uint16 rsvd4:2;                     
    Uint16 GPIO237:2;                   
    Uint16 GPIO238:2;                   
    Uint16 GPIO239:2;                   
};

union GPHGMUX1_REG {
    Uint32  all;
    struct  GPHGMUX1_BITS  bit;
};

struct GPHGMUX2_BITS {                  
    Uint16 GPIO240:2;                   
    Uint16 GPIO241:2;                   
    Uint16 GPIO242:2;                   
    Uint16 rsvd1:2;                     
    Uint16 GPIO244:2;                   
    Uint16 GPIO245:2;                   
    Uint16 rsvd2:2;                     
    Uint16 rsvd3:2;                     
    Uint16 rsvd4:2;                     
    Uint16 rsvd5:2;                     
    Uint16 rsvd6:2;                     
    Uint16 rsvd7:2;                     
    Uint16 GPIO252:2;                   
    Uint16 rsvd8:2;                     
    Uint16 rsvd9:2;                     
    Uint16 rsvd10:2;                    
};

union GPHGMUX2_REG {
    Uint32  all;
    struct  GPHGMUX2_BITS  bit;
};

struct GPHCSEL1_BITS {                  
    Uint16 GPIO224:4;                   
    Uint16 GPIO225:4;                   
    Uint16 GPIO226:4;                   
    Uint16 GPIO227:4;                   
    Uint16 GPIO228:4;                   
    Uint16 GPIO229:4;                   
    Uint16 GPIO230:4;                   
    Uint16 GPIO231:4;                   
};

union GPHCSEL1_REG {
    Uint32  all;
    struct  GPHCSEL1_BITS  bit;
};

struct GPHCSEL2_BITS {                  
    Uint16 GPIO232:4;                   
    Uint16 GPIO233:4;                   
    Uint16 rsvd1:4;                     
    Uint16 rsvd2:4;                     
    Uint16 GPIO236:4;                   
    Uint16 GPIO237:4;                   
    Uint16 GPIO238:4;                   
    Uint16 GPIO239:4;                   
};

union GPHCSEL2_REG {
    Uint32  all;
    struct  GPHCSEL2_BITS  bit;
};

struct GPHCSEL3_BITS {                  
    Uint16 GPIO240:4;                   
    Uint16 GPIO241:4;                   
    Uint16 GPIO242:4;                   
    Uint16 rsvd1:4;                     
    Uint16 GPIO244:4;                   
    Uint16 GPIO245:4;                   
    Uint16 rsvd2:4;                     
    Uint16 GPIO247:4;                   
};

union GPHCSEL3_REG {
    Uint32  all;
    struct  GPHCSEL3_BITS  bit;
};

struct GPHCSEL4_BITS {                  
    Uint16 GPIO248:4;                   
    Uint16 GPIO249:4;                   
    Uint16 rsvd1:4;                     
    Uint16 GPIO251:4;                   
    Uint16 GPIO252:4;                   
    Uint16 GPIO253:4;                   
    Uint16 rsvd2:4;                     
    Uint16 rsvd3:4;                     
};

union GPHCSEL4_REG {
    Uint32  all;
    struct  GPHCSEL4_BITS  bit;
};

struct GPHLOCK_BITS {                   
    Uint16 GPIO224:1;                   
    Uint16 GPIO225:1;                   
    Uint16 GPIO226:1;                   
    Uint16 GPIO227:1;                   
    Uint16 GPIO228:1;                   
    Uint16 GPIO229:1;                   
    Uint16 GPIO230:1;                   
    Uint16 GPIO231:1;                   
    Uint16 GPIO232:1;                   
    Uint16 GPIO233:1;                   
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 GPIO236:1;                   
    Uint16 GPIO237:1;                   
    Uint16 GPIO238:1;                   
    Uint16 GPIO239:1;                   
    Uint16 GPIO240:1;                   
    Uint16 GPIO241:1;                   
    Uint16 GPIO242:1;                   
    Uint16 rsvd3:1;                     
    Uint16 GPIO244:1;                   
    Uint16 GPIO245:1;                   
    Uint16 rsvd4:1;                     
    Uint16 GPIO247:1;                   
    Uint16 GPIO248:1;                   
    Uint16 GPIO249:1;                   
    Uint16 rsvd5:1;                     
    Uint16 GPIO251:1;                   
    Uint16 GPIO252:1;                   
    Uint16 GPIO253:1;                   
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:1;                     
};

union GPHLOCK_REG {
    Uint32  all;
    struct  GPHLOCK_BITS  bit;
};

struct GPHCR_BITS {                     
    Uint16 GPIO224:1;                   
    Uint16 GPIO225:1;                   
    Uint16 GPIO226:1;                   
    Uint16 GPIO227:1;                   
    Uint16 GPIO228:1;                   
    Uint16 GPIO229:1;                   
    Uint16 GPIO230:1;                   
    Uint16 GPIO231:1;                   
    Uint16 GPIO232:1;                   
    Uint16 GPIO233:1;                   
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 GPIO236:1;                   
    Uint16 GPIO237:1;                   
    Uint16 GPIO238:1;                   
    Uint16 GPIO239:1;                   
    Uint16 GPIO240:1;                   
    Uint16 GPIO241:1;                   
    Uint16 GPIO242:1;                   
    Uint16 rsvd3:1;                     
    Uint16 GPIO244:1;                   
    Uint16 GPIO245:1;                   
    Uint16 rsvd4:1;                     
    Uint16 GPIO247:1;                   
    Uint16 GPIO248:1;                   
    Uint16 GPIO249:1;                   
    Uint16 rsvd5:1;                     
    Uint16 GPIO251:1;                   
    Uint16 GPIO252:1;                   
    Uint16 GPIO253:1;                   
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:1;                     
};

union GPHCR_REG {
    Uint32  all;
    struct  GPHCR_BITS  bit;
};

struct GPIO_CTRL_REGS {
    union   GPACTRL_REG                      GPACTRL;                      
    union   GPAQSEL1_REG                     GPAQSEL1;                     
    union   GPAQSEL2_REG                     GPAQSEL2;                     
    union   GPAMUX1_REG                      GPAMUX1;                      
    union   GPAMUX2_REG                      GPAMUX2;                      
    union   GPADIR_REG                       GPADIR;                       
    union   GPAPUD_REG                       GPAPUD;                       
    Uint16                                   rsvd1[2];                     
    union   GPAINV_REG                       GPAINV;                       
    union   GPAODR_REG                       GPAODR;                       
    union   GPAAMSEL_REG                     GPAAMSEL;                     
    Uint16                                   rsvd2[10];                    
    union   GPAGMUX1_REG                     GPAGMUX1;                     
    union   GPAGMUX2_REG                     GPAGMUX2;                     
    Uint16                                   rsvd3[4];                     
    union   GPACSEL1_REG                     GPACSEL1;                     
    union   GPACSEL2_REG                     GPACSEL2;                     
    union   GPACSEL3_REG                     GPACSEL3;                     
    union   GPACSEL4_REG                     GPACSEL4;                     
    Uint16                                   rsvd4[12];                    
    union   GPALOCK_REG                      GPALOCK;                      
    union   GPACR_REG                        GPACR;                        
    union   GPBCTRL_REG                      GPBCTRL;                      
    union   GPBQSEL1_REG                     GPBQSEL1;                     
    union   GPBQSEL2_REG                     GPBQSEL2;                     
    union   GPBMUX1_REG                      GPBMUX1;                      
    union   GPBMUX2_REG                      GPBMUX2;                      
    union   GPBDIR_REG                       GPBDIR;                       
    union   GPBPUD_REG                       GPBPUD;                       
    Uint16                                   rsvd5[2];                     
    union   GPBINV_REG                       GPBINV;                       
    union   GPBODR_REG                       GPBODR;                       
    Uint16                                   rsvd6[12];                    
    union   GPBGMUX1_REG                     GPBGMUX1;                     
    union   GPBGMUX2_REG                     GPBGMUX2;                     
    Uint16                                   rsvd7[4];                     
    union   GPBCSEL1_REG                     GPBCSEL1;                     
    union   GPBCSEL2_REG                     GPBCSEL2;                     
    union   GPBCSEL3_REG                     GPBCSEL3;                     
    union   GPBCSEL4_REG                     GPBCSEL4;                     
    Uint16                                   rsvd8[12];                    
    union   GPBLOCK_REG                      GPBLOCK;                      
    union   GPBCR_REG                        GPBCR;                        
    Uint16                                   rsvd9[320];                   
    union   GPHCTRL_REG                      GPHCTRL;                      
    union   GPHQSEL1_REG                     GPHQSEL1;                     
    union   GPHQSEL2_REG                     GPHQSEL2;                     
    union   GPHMUX1_REG                      GPHMUX1;                      
    union   GPHMUX2_REG                      GPHMUX2;                      
    Uint16                                   rsvd10[6];                    
    union   GPHINV_REG                       GPHINV;                       
    Uint16                                   rsvd11[2];                    
    union   GPHAMSEL_REG                     GPHAMSEL;                     
    Uint16                                   rsvd12[10];                   
    union   GPHGMUX1_REG                     GPHGMUX1;                     
    union   GPHGMUX2_REG                     GPHGMUX2;                     
    Uint16                                   rsvd13[4];                    
    union   GPHCSEL1_REG                     GPHCSEL1;                     
    union   GPHCSEL2_REG                     GPHCSEL2;                     
    union   GPHCSEL3_REG                     GPHCSEL3;                     
    union   GPHCSEL4_REG                     GPHCSEL4;                     
    Uint16                                   rsvd14[12];                   
    union   GPHLOCK_REG                      GPHLOCK;                      
    union   GPHCR_REG                        GPHCR;                        
};

struct GPADAT_BITS {                    
    Uint16 GPIO0:1;                     
    Uint16 GPIO1:1;                     
    Uint16 GPIO2:1;                     
    Uint16 GPIO3:1;                     
    Uint16 GPIO4:1;                     
    Uint16 GPIO5:1;                     
    Uint16 GPIO6:1;                     
    Uint16 GPIO7:1;                     
    Uint16 GPIO8:1;                     
    Uint16 GPIO9:1;                     
    Uint16 GPIO10:1;                    
    Uint16 GPIO11:1;                    
    Uint16 GPIO12:1;                    
    Uint16 GPIO13:1;                    
    Uint16 GPIO14:1;                    
    Uint16 GPIO15:1;                    
    Uint16 GPIO16:1;                    
    Uint16 GPIO17:1;                    
    Uint16 GPIO18:1;                    
    Uint16 GPIO19:1;                    
    Uint16 GPIO20:1;                    
    Uint16 GPIO21:1;                    
    Uint16 GPIO22:1;                    
    Uint16 GPIO23:1;                    
    Uint16 GPIO24:1;                    
    Uint16 GPIO25:1;                    
    Uint16 GPIO26:1;                    
    Uint16 GPIO27:1;                    
    Uint16 GPIO28:1;                    
    Uint16 GPIO29:1;                    
    Uint16 GPIO30:1;                    
    Uint16 GPIO31:1;                    
};

union GPADAT_REG {
    Uint32  all;
    struct  GPADAT_BITS  bit;
};

struct GPASET_BITS {                    
    Uint16 GPIO0:1;                     
    Uint16 GPIO1:1;                     
    Uint16 GPIO2:1;                     
    Uint16 GPIO3:1;                     
    Uint16 GPIO4:1;                     
    Uint16 GPIO5:1;                     
    Uint16 GPIO6:1;                     
    Uint16 GPIO7:1;                     
    Uint16 GPIO8:1;                     
    Uint16 GPIO9:1;                     
    Uint16 GPIO10:1;                    
    Uint16 GPIO11:1;                    
    Uint16 GPIO12:1;                    
    Uint16 GPIO13:1;                    
    Uint16 GPIO14:1;                    
    Uint16 GPIO15:1;                    
    Uint16 GPIO16:1;                    
    Uint16 GPIO17:1;                    
    Uint16 GPIO18:1;                    
    Uint16 GPIO19:1;                    
    Uint16 GPIO20:1;                    
    Uint16 GPIO21:1;                    
    Uint16 GPIO22:1;                    
    Uint16 GPIO23:1;                    
    Uint16 GPIO24:1;                    
    Uint16 GPIO25:1;                    
    Uint16 GPIO26:1;                    
    Uint16 GPIO27:1;                    
    Uint16 GPIO28:1;                    
    Uint16 GPIO29:1;                    
    Uint16 GPIO30:1;                    
    Uint16 GPIO31:1;                    
};

union GPASET_REG {
    Uint32  all;
    struct  GPASET_BITS  bit;
};

struct GPACLEAR_BITS {                  
    Uint16 GPIO0:1;                     
    Uint16 GPIO1:1;                     
    Uint16 GPIO2:1;                     
    Uint16 GPIO3:1;                     
    Uint16 GPIO4:1;                     
    Uint16 GPIO5:1;                     
    Uint16 GPIO6:1;                     
    Uint16 GPIO7:1;                     
    Uint16 GPIO8:1;                     
    Uint16 GPIO9:1;                     
    Uint16 GPIO10:1;                    
    Uint16 GPIO11:1;                    
    Uint16 GPIO12:1;                    
    Uint16 GPIO13:1;                    
    Uint16 GPIO14:1;                    
    Uint16 GPIO15:1;                    
    Uint16 GPIO16:1;                    
    Uint16 GPIO17:1;                    
    Uint16 GPIO18:1;                    
    Uint16 GPIO19:1;                    
    Uint16 GPIO20:1;                    
    Uint16 GPIO21:1;                    
    Uint16 GPIO22:1;                    
    Uint16 GPIO23:1;                    
    Uint16 GPIO24:1;                    
    Uint16 GPIO25:1;                    
    Uint16 GPIO26:1;                    
    Uint16 GPIO27:1;                    
    Uint16 GPIO28:1;                    
    Uint16 GPIO29:1;                    
    Uint16 GPIO30:1;                    
    Uint16 GPIO31:1;                    
};

union GPACLEAR_REG {
    Uint32  all;
    struct  GPACLEAR_BITS  bit;
};

struct GPATOGGLE_BITS {                 
    Uint16 GPIO0:1;                     
    Uint16 GPIO1:1;                     
    Uint16 GPIO2:1;                     
    Uint16 GPIO3:1;                     
    Uint16 GPIO4:1;                     
    Uint16 GPIO5:1;                     
    Uint16 GPIO6:1;                     
    Uint16 GPIO7:1;                     
    Uint16 GPIO8:1;                     
    Uint16 GPIO9:1;                     
    Uint16 GPIO10:1;                    
    Uint16 GPIO11:1;                    
    Uint16 GPIO12:1;                    
    Uint16 GPIO13:1;                    
    Uint16 GPIO14:1;                    
    Uint16 GPIO15:1;                    
    Uint16 GPIO16:1;                    
    Uint16 GPIO17:1;                    
    Uint16 GPIO18:1;                    
    Uint16 GPIO19:1;                    
    Uint16 GPIO20:1;                    
    Uint16 GPIO21:1;                    
    Uint16 GPIO22:1;                    
    Uint16 GPIO23:1;                    
    Uint16 GPIO24:1;                    
    Uint16 GPIO25:1;                    
    Uint16 GPIO26:1;                    
    Uint16 GPIO27:1;                    
    Uint16 GPIO28:1;                    
    Uint16 GPIO29:1;                    
    Uint16 GPIO30:1;                    
    Uint16 GPIO31:1;                    
};

union GPATOGGLE_REG {
    Uint32  all;
    struct  GPATOGGLE_BITS  bit;
};

struct GPBDAT_BITS {                    
    Uint16 GPIO32:1;                    
    Uint16 GPIO33:1;                    
    Uint16 GPIO34:1;                    
    Uint16 GPIO35:1;                    
    Uint16 rsvd1:1;                     
    Uint16 GPIO37:1;                    
    Uint16 rsvd2:1;                     
    Uint16 GPIO39:1;                    
    Uint16 GPIO40:1;                    
    Uint16 GPIO41:1;                    
    Uint16 GPIO42:1;                    
    Uint16 GPIO43:1;                    
    Uint16 GPIO44:1;                    
    Uint16 GPIO45:1;                    
    Uint16 GPIO46:1;                    
    Uint16 GPIO47:1;                    
    Uint16 GPIO48:1;                    
    Uint16 GPIO49:1;                    
    Uint16 GPIO50:1;                    
    Uint16 GPIO51:1;                    
    Uint16 GPIO52:1;                    
    Uint16 GPIO53:1;                    
    Uint16 GPIO54:1;                    
    Uint16 GPIO55:1;                    
    Uint16 GPIO56:1;                    
    Uint16 GPIO57:1;                    
    Uint16 GPIO58:1;                    
    Uint16 GPIO59:1;                    
    Uint16 GPIO60:1;                    
    Uint16 GPIO61:1;                    
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
};

union GPBDAT_REG {
    Uint32  all;
    struct  GPBDAT_BITS  bit;
};

struct GPBSET_BITS {                    
    Uint16 GPIO32:1;                    
    Uint16 GPIO33:1;                    
    Uint16 GPIO34:1;                    
    Uint16 GPIO35:1;                    
    Uint16 rsvd1:1;                     
    Uint16 GPIO37:1;                    
    Uint16 rsvd2:1;                     
    Uint16 GPIO39:1;                    
    Uint16 GPIO40:1;                    
    Uint16 GPIO41:1;                    
    Uint16 GPIO42:1;                    
    Uint16 GPIO43:1;                    
    Uint16 GPIO44:1;                    
    Uint16 GPIO45:1;                    
    Uint16 GPIO46:1;                    
    Uint16 GPIO47:1;                    
    Uint16 GPIO48:1;                    
    Uint16 GPIO49:1;                    
    Uint16 GPIO50:1;                    
    Uint16 GPIO51:1;                    
    Uint16 GPIO52:1;                    
    Uint16 GPIO53:1;                    
    Uint16 GPIO54:1;                    
    Uint16 GPIO55:1;                    
    Uint16 GPIO56:1;                    
    Uint16 GPIO57:1;                    
    Uint16 GPIO58:1;                    
    Uint16 GPIO59:1;                    
    Uint16 GPIO60:1;                    
    Uint16 GPIO61:1;                    
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
};

union GPBSET_REG {
    Uint32  all;
    struct  GPBSET_BITS  bit;
};

struct GPBCLEAR_BITS {                  
    Uint16 GPIO32:1;                    
    Uint16 GPIO33:1;                    
    Uint16 GPIO34:1;                    
    Uint16 GPIO35:1;                    
    Uint16 rsvd1:1;                     
    Uint16 GPIO37:1;                    
    Uint16 rsvd2:1;                     
    Uint16 GPIO39:1;                    
    Uint16 GPIO40:1;                    
    Uint16 GPIO41:1;                    
    Uint16 GPIO42:1;                    
    Uint16 GPIO43:1;                    
    Uint16 GPIO44:1;                    
    Uint16 GPIO45:1;                    
    Uint16 GPIO46:1;                    
    Uint16 GPIO47:1;                    
    Uint16 GPIO48:1;                    
    Uint16 GPIO49:1;                    
    Uint16 GPIO50:1;                    
    Uint16 GPIO51:1;                    
    Uint16 GPIO52:1;                    
    Uint16 GPIO53:1;                    
    Uint16 GPIO54:1;                    
    Uint16 GPIO55:1;                    
    Uint16 GPIO56:1;                    
    Uint16 GPIO57:1;                    
    Uint16 GPIO58:1;                    
    Uint16 GPIO59:1;                    
    Uint16 GPIO60:1;                    
    Uint16 GPIO61:1;                    
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
};

union GPBCLEAR_REG {
    Uint32  all;
    struct  GPBCLEAR_BITS  bit;
};

struct GPBTOGGLE_BITS {                 
    Uint16 GPIO32:1;                    
    Uint16 GPIO33:1;                    
    Uint16 GPIO34:1;                    
    Uint16 GPIO35:1;                    
    Uint16 rsvd1:1;                     
    Uint16 GPIO37:1;                    
    Uint16 rsvd2:1;                     
    Uint16 GPIO39:1;                    
    Uint16 GPIO40:1;                    
    Uint16 GPIO41:1;                    
    Uint16 GPIO42:1;                    
    Uint16 GPIO43:1;                    
    Uint16 GPIO44:1;                    
    Uint16 GPIO45:1;                    
    Uint16 GPIO46:1;                    
    Uint16 GPIO47:1;                    
    Uint16 GPIO48:1;                    
    Uint16 GPIO49:1;                    
    Uint16 GPIO50:1;                    
    Uint16 GPIO51:1;                    
    Uint16 GPIO52:1;                    
    Uint16 GPIO53:1;                    
    Uint16 GPIO54:1;                    
    Uint16 GPIO55:1;                    
    Uint16 GPIO56:1;                    
    Uint16 GPIO57:1;                    
    Uint16 GPIO58:1;                    
    Uint16 GPIO59:1;                    
    Uint16 GPIO60:1;                    
    Uint16 GPIO61:1;                    
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
};

union GPBTOGGLE_REG {
    Uint32  all;
    struct  GPBTOGGLE_BITS  bit;
};

struct GPHDAT_BITS {                    
    Uint16 GPIO224:1;                   
    Uint16 GPIO225:1;                   
    Uint16 GPIO226:1;                   
    Uint16 GPIO227:1;                   
    Uint16 GPIO228:1;                   
    Uint16 GPIO229:1;                   
    Uint16 GPIO230:1;                   
    Uint16 GPIO231:1;                   
    Uint16 GPIO232:1;                   
    Uint16 GPIO233:1;                   
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 GPIO236:1;                   
    Uint16 GPIO237:1;                   
    Uint16 GPIO238:1;                   
    Uint16 GPIO239:1;                   
    Uint16 GPIO240:1;                   
    Uint16 GPIO241:1;                   
    Uint16 GPIO242:1;                   
    Uint16 rsvd3:1;                     
    Uint16 GPIO244:1;                   
    Uint16 GPIO245:1;                   
    Uint16 rsvd4:1;                     
    Uint16 GPIO247:1;                   
    Uint16 GPIO248:1;                   
    Uint16 GPIO249:1;                   
    Uint16 rsvd5:1;                     
    Uint16 GPIO251:1;                   
    Uint16 GPIO252:1;                   
    Uint16 GPIO253:1;                   
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:1;                     
};

union GPHDAT_REG {
    Uint32  all;
    struct  GPHDAT_BITS  bit;
};

struct GPIO_DATA_REGS {
    union   GPADAT_REG                       GPADAT;                       
    union   GPASET_REG                       GPASET;                       
    union   GPACLEAR_REG                     GPACLEAR;                     
    union   GPATOGGLE_REG                    GPATOGGLE;                    
    union   GPBDAT_REG                       GPBDAT;                       
    union   GPBSET_REG                       GPBSET;                       
    union   GPBCLEAR_REG                     GPBCLEAR;                     
    union   GPBTOGGLE_REG                    GPBTOGGLE;                    
    Uint16                                   rsvd1[40];                    
    union   GPHDAT_REG                       GPHDAT;                       
};

struct GPIO_DATA_READ_REGS {
    Uint32                                   GPADAT_R;                     
    Uint32                                   GPBDAT_R;                     
    Uint16                                   rsvd1[10];                    
    Uint32                                   GPHDAT_R;                     
};




extern volatile struct GPIO_CTRL_REGS GpioCtrlRegs;
extern volatile struct GPIO_DATA_REGS GpioDataRegs;
extern volatile struct GPIO_DATA_READ_REGS GpioDataReadRegs;



















































struct HICREV_BITS {                    
    Uint16 MINOR:6;                     
    Uint16 CUSTOM:2;                    
    Uint16 MAJOR:3;                     
    Uint16 RTL:5;                       
    Uint16 FUNC:12;                     
    Uint16 rsvd1:2;                     
    Uint16 SCHEME:2;                    
};

union HICREV_REG {
    Uint32  all;
    struct  HICREV_BITS  bit;
};

struct HICGCR_BITS {                    
    Uint16 HICEN:4;                     
    Uint16 rsvd1:12;                    
    Uint16 rsvd2:16;                    
};

union HICGCR_REG {
    Uint32  all;
    struct  HICGCR_BITS  bit;
};

struct HICLOCK_BITS {                   
    Uint16 LOCK:1;                      
    Uint16 rsvd1:15;                    
    Uint16 WRITE_ENABLE_KEY:16;         
};

union HICLOCK_REG {
    Uint32  all;
    struct  HICLOCK_BITS  bit;
};

struct HICMODECR_BITS {                 
    Uint16 DW_MODE:2;                   
    Uint16 rsvd1:2;                     
    Uint16 RW_MODE:1;                   
    Uint16 BEN_PRESENT:1;               
    Uint16 RDY_PRESENT:1;               
    Uint16 rsvd2:1;                     
    Uint16 H2DBUF_DEVWREN:1;            
    Uint16 D2HBUF_HOSTWREN:1;           
    Uint16 EN_DEVACC:1;                 
    Uint16 EN_HOSTWREALLOW:1;           
    Uint16 rsvd3:4;                     
    Uint16 rsvd4:16;                    
};

union HICMODECR_REG {
    Uint32  all;
    struct  HICMODECR_BITS  bit;
};

struct HICPINPOLCR_BITS {               
    Uint16 CS_POL:1;                    
    Uint16 BEN_POL:1;                   
    Uint16 OE_POL:1;                    
    Uint16 WE_POL:1;                    
    Uint16 RDY_POL:1;                   
    Uint16 rsvd1:11;                    
    Uint16 rsvd2:16;                    
};

union HICPINPOLCR_REG {
    Uint32  all;
    struct  HICPINPOLCR_BITS  bit;
};

struct HICBASESEL_BITS {                
    Uint16 BASE_SELECT:3;               
    Uint16 rsvd1:13;                    
    Uint16 rsvd2:16;                    
};

union HICBASESEL_REG {
    Uint32  all;
    struct  HICBASESEL_BITS  bit;
};

struct HICHOSTCR_BITS {                 
    Uint16 EALLOW_EN:1;                 
    Uint16 ACCSIZE:1;                   
    Uint16 PAGESEL:1;                   
    Uint16 rsvd1:5;                     
    Uint16 HKEY:8;                      
    Uint16 rsvd2:16;                    
};

union HICHOSTCR_REG {
    Uint32  all;
    struct  HICHOSTCR_BITS  bit;
};

struct HICERRADDR_BITS {                
    Uint16 H2D_ERR_ADDR:8;              
    Uint16 rsvd1:4;                     
    Uint16 H2D_BASE_SEL:3;              
    Uint16 rsvd2:1;                     
    Uint16 D2H_ERR_ADDR:8;              
    Uint16 rsvd3:4;                     
    Uint16 D2H_BASE_SEL:3;              
    Uint16 rsvd4:1;                     
};

union HICERRADDR_REG {
    Uint32  all;
    struct  HICERRADDR_BITS  bit;
};

struct HICDBADDR0_BITS {                
    Uint16 rsvd1:7;                     
    Uint32 BASE_ADDR:25;                
};

union HICDBADDR0_REG {
    Uint32  all;
    struct  HICDBADDR0_BITS  bit;
};

struct HICDBADDR1_BITS {                
    Uint16 rsvd1:7;                     
    Uint32 BASE_ADDR:25;                
};

union HICDBADDR1_REG {
    Uint32  all;
    struct  HICDBADDR1_BITS  bit;
};

struct HICDBADDR2_BITS {                
    Uint16 rsvd1:7;                     
    Uint32 BASE_ADDR:25;                
};

union HICDBADDR2_REG {
    Uint32  all;
    struct  HICDBADDR2_BITS  bit;
};

struct HICDBADDR3_BITS {                
    Uint16 rsvd1:7;                     
    Uint32 BASE_ADDR:25;                
};

union HICDBADDR3_REG {
    Uint32  all;
    struct  HICDBADDR3_BITS  bit;
};

struct HICDBADDR4_BITS {                
    Uint16 rsvd1:7;                     
    Uint32 BASE_ADDR:25;                
};

union HICDBADDR4_REG {
    Uint32  all;
    struct  HICDBADDR4_BITS  bit;
};

struct HICDBADDR5_BITS {                
    Uint16 rsvd1:7;                     
    Uint32 BASE_ADDR:25;                
};

union HICDBADDR5_REG {
    Uint32  all;
    struct  HICDBADDR5_BITS  bit;
};

struct HICDBADDR6_BITS {                
    Uint16 rsvd1:7;                     
    Uint32 BASE_ADDR:25;                
};

union HICDBADDR6_REG {
    Uint32  all;
    struct  HICDBADDR6_BITS  bit;
};

struct HICDBADDR7_BITS {                
    Uint16 rsvd1:7;                     
    Uint32 BASE_ADDR:25;                
};

union HICDBADDR7_REG {
    Uint32  all;
    struct  HICDBADDR7_BITS  bit;
};

struct HICH2DINTEN_BITS {               
    Uint16 H2D_INTEN:1;                 
    Uint16 BUSERR_INTEN:1;              
    Uint16 ILLWR_INTEN:1;               
    Uint16 ILLRD_INTEN:1;               
    Uint16 rsvd1:12;                    
    Uint16 rsvd2:16;                    
};

union HICH2DINTEN_REG {
    Uint32  all;
    struct  HICH2DINTEN_BITS  bit;
};

struct HICH2DINTFLG_BITS {              
    Uint16 H2D_FLG:1;                   
    Uint16 BUSERR_FLG:1;                
    Uint16 ILLWR_FLG:1;                 
    Uint16 ILLRD_FLG:1;                 
    Uint16 rsvd1:12;                    
    Uint16 rsvd2:16;                    
};

union HICH2DINTFLG_REG {
    Uint32  all;
    struct  HICH2DINTFLG_BITS  bit;
};

struct HICH2DINTCLR_BITS {              
    Uint16 H2D_CLR:1;                   
    Uint16 BUSERR_CLR:1;                
    Uint16 ILLWR_CLR:1;                 
    Uint16 ILLRD_CLR:1;                 
    Uint16 rsvd1:12;                    
    Uint16 rsvd2:16;                    
};

union HICH2DINTCLR_REG {
    Uint32  all;
    struct  HICH2DINTCLR_BITS  bit;
};

struct HICH2DINTFRC_BITS {              
    Uint16 H2D_INTFRC:1;                
    Uint16 BUSERR_INTFRC:1;             
    Uint16 ILLWR_INTFRC:1;              
    Uint16 ILLRD_INTFRC:1;              
    Uint16 rsvd1:12;                    
    Uint16 rsvd2:16;                    
};

union HICH2DINTFRC_REG {
    Uint32  all;
    struct  HICH2DINTFRC_BITS  bit;
};

struct HICD2HINTEN_BITS {               
    Uint16 D2H_INTEN:1;                 
    Uint16 BUSERR_INTEN:1;              
    Uint16 ILLWR_INTEN:1;               
    Uint16 ILLRD_INTEN:1;               
    Uint16 ACCVIO_INTEN:1;              
    Uint16 rsvd1:11;                    
    Uint16 EVTRIG_INTEN:16;             
};

union HICD2HINTEN_REG {
    Uint32  all;
    struct  HICD2HINTEN_BITS  bit;
};

struct HICD2HINTFLG_BITS {              
    Uint16 D2H_FLG:1;                   
    Uint16 BUSERR_FLG:1;                
    Uint16 ILLWR_FLG:1;                 
    Uint16 ILLRD_FLG:1;                 
    Uint16 ACCVIO_FLG:1;                
    Uint16 rsvd1:11;                    
    Uint16 EVTRIG_FLG:16;               
};

union HICD2HINTFLG_REG {
    Uint32  all;
    struct  HICD2HINTFLG_BITS  bit;
};

struct HICD2HINTCLR_BITS {              
    Uint16 D2H_CLR:1;                   
    Uint16 BUSERR_CLR:1;                
    Uint16 ILLWR_CLR:1;                 
    Uint16 ILLRD_CLR:1;                 
    Uint16 ACCVIO_CLR:1;                
    Uint16 rsvd1:11;                    
    Uint16 EVTRIG_CLR:16;               
};

union HICD2HINTCLR_REG {
    Uint32  all;
    struct  HICD2HINTCLR_BITS  bit;
};

struct HICD2HINTFRC_BITS {              
    Uint16 D2H_INTFRC:1;                
    Uint16 BUSERR_INTFRC:1;             
    Uint16 ILLWR_INTFRC:1;              
    Uint16 ILLRD_INTFRC:1;              
    Uint16 ACCVIO_INTFRC:1;             
    Uint16 rsvd1:11;                    
    Uint16 EVTRIG_INTFRC:16;            
};

union HICD2HINTFRC_REG {
    Uint32  all;
    struct  HICD2HINTFRC_BITS  bit;
};

struct HICCOMMIT_BITS {                 
    Uint16 COMMIT:1;                    
    Uint16 rsvd1:15;                    
    Uint16 WRITE_ENABLE_KEY:16;         
};

union HICCOMMIT_REG {
    Uint32  all;
    struct  HICCOMMIT_BITS  bit;
};

struct HIC_CFG_REGS {
    union   HICREV_REG                       HICREV;                       
    union   HICGCR_REG                       HICGCR;                       
    union   HICLOCK_REG                      HICLOCK;                      
    union   HICMODECR_REG                    HICMODECR;                    
    union   HICPINPOLCR_REG                  HICPINPOLCR;                  
    union   HICBASESEL_REG                   HICBASESEL;                   
    union   HICHOSTCR_REG                    HICHOSTCR;                    
    union   HICERRADDR_REG                   HICERRADDR;                   
    Uint32                                   HICH2DTOKEN;                  
    Uint32                                   HICD2HTOKEN;                  
    union   HICDBADDR0_REG                   HICDBADDR0;                   
    union   HICDBADDR1_REG                   HICDBADDR1;                   
    union   HICDBADDR2_REG                   HICDBADDR2;                   
    union   HICDBADDR3_REG                   HICDBADDR3;                   
    union   HICDBADDR4_REG                   HICDBADDR4;                   
    union   HICDBADDR5_REG                   HICDBADDR5;                   
    union   HICDBADDR6_REG                   HICDBADDR6;                   
    union   HICDBADDR7_REG                   HICDBADDR7;                   
    Uint16                                   rsvd1[4];                     
    union   HICH2DINTEN_REG                  HICH2DINTEN;                  
    union   HICH2DINTFLG_REG                 HICH2DINTFLG;                 
    union   HICH2DINTCLR_REG                 HICH2DINTCLR;                 
    union   HICH2DINTFRC_REG                 HICH2DINTFRC;                 
    union   HICD2HINTEN_REG                  HICD2HINTEN;                  
    union   HICD2HINTFLG_REG                 HICD2HINTFLG;                 
    union   HICD2HINTCLR_REG                 HICD2HINTCLR;                 
    union   HICD2HINTFRC_REG                 HICD2HINTFRC;                 
    Uint32                                   HICACCVIOADDR;                
    union   HICCOMMIT_REG                    HICCOMMIT;                    
    Uint16                                   rsvd2[4];                     
    Uint32                                   H2D_BUF0;                     
    Uint32                                   H2D_BUF1;                     
    Uint32                                   H2D_BUF2;                     
    Uint32                                   H2D_BUF3;                     
    Uint32                                   H2D_BUF4;                     
    Uint32                                   H2D_BUF5;                     
    Uint32                                   H2D_BUF6;                     
    Uint32                                   H2D_BUF7;                     
    Uint32                                   H2D_BUF8;                     
    Uint32                                   H2D_BUF9;                     
    Uint32                                   H2D_BUF10;                    
    Uint32                                   H2D_BUF11;                    
    Uint32                                   H2D_BUF12;                    
    Uint32                                   H2D_BUF13;                    
    Uint32                                   H2D_BUF14;                    
    Uint32                                   H2D_BUF15;                    
    Uint32                                   D2H_BUF0;                     
    Uint32                                   D2H_BUF1;                     
    Uint32                                   D2H_BUF2;                     
    Uint32                                   D2H_BUF3;                     
    Uint32                                   D2H_BUF4;                     
    Uint32                                   D2H_BUF5;                     
    Uint32                                   D2H_BUF6;                     
    Uint32                                   D2H_BUF7;                     
    Uint32                                   D2H_BUF8;                     
    Uint32                                   D2H_BUF9;                     
    Uint32                                   D2H_BUF10;                    
    Uint32                                   D2H_BUF11;                    
    Uint32                                   D2H_BUF12;                    
    Uint32                                   D2H_BUF13;                    
    Uint32                                   D2H_BUF14;                    
    Uint32                                   D2H_BUF15;                    
};




extern volatile struct HIC_CFG_REGS HicRegs;



















































struct I2COAR_BITS {                    
    Uint16 OAR:10;                      
    Uint16 rsvd1:6;                     
};

union I2COAR_REG {
    Uint16  all;
    struct  I2COAR_BITS  bit;
};

struct I2CIER_BITS {                    
    Uint16 ARBL:1;                      
    Uint16 NACK:1;                      
    Uint16 ARDY:1;                      
    Uint16 RRDY:1;                      
    Uint16 XRDY:1;                      
    Uint16 SCD:1;                       
    Uint16 AAS:1;                       
    Uint16 rsvd1:9;                     
};

union I2CIER_REG {
    Uint16  all;
    struct  I2CIER_BITS  bit;
};

struct I2CSTR_BITS {                    
    Uint16 ARBL:1;                      
    Uint16 NACK:1;                      
    Uint16 ARDY:1;                      
    Uint16 RRDY:1;                      
    Uint16 XRDY:1;                      
    Uint16 SCD:1;                       
    Uint16 BYTESENT:1;                  
    Uint16 rsvd1:1;                     
    Uint16 AD0:1;                       
    Uint16 AAS:1;                       
    Uint16 XSMT:1;                      
    Uint16 RSFULL:1;                    
    Uint16 BB:1;                        
    Uint16 NACKSNT:1;                   
    Uint16 SDIR:1;                      
    Uint16 rsvd2:1;                     
};

union I2CSTR_REG {
    Uint16  all;
    struct  I2CSTR_BITS  bit;
};

struct I2CDRR_BITS {                    
    Uint16 DATA:8;                      
    Uint16 rsvd1:8;                     
};

union I2CDRR_REG {
    Uint16  all;
    struct  I2CDRR_BITS  bit;
};

struct I2CSAR_BITS {                    
    Uint16 SAR:10;                      
    Uint16 rsvd1:6;                     
};

union I2CSAR_REG {
    Uint16  all;
    struct  I2CSAR_BITS  bit;
};

struct I2CDXR_BITS {                    
    Uint16 DATA:8;                      
    Uint16 rsvd1:8;                     
};

union I2CDXR_REG {
    Uint16  all;
    struct  I2CDXR_BITS  bit;
};

struct I2CMDR_BITS {                    
    Uint16 BC:3;                        
    Uint16 FDF:1;                       
    Uint16 STB:1;                       
    Uint16 IRS:1;                       
    Uint16 DLB:1;                       
    Uint16 RM:1;                        
    Uint16 XA:1;                        
    Uint16 TRX:1;                       
    Uint16 MST:1;                       
    Uint16 STP:1;                       
    Uint16 rsvd1:1;                     
    Uint16 STT:1;                       
    Uint16 FREE:1;                      
    Uint16 NACKMOD:1;                   
};

union I2CMDR_REG {
    Uint16  all;
    struct  I2CMDR_BITS  bit;
};

struct I2CISRC_BITS {                   
    Uint16 INTCODE:3;                   
    Uint16 rsvd1:5;                     
    Uint16 WRITE_ZEROS:4;               
    Uint16 rsvd2:4;                     
};

union I2CISRC_REG {
    Uint16  all;
    struct  I2CISRC_BITS  bit;
};

struct I2CEMDR_BITS {                   
    Uint16 BC:1;                        
    Uint16 FCM:1;                       
    Uint16 rsvd1:14;                    
};

union I2CEMDR_REG {
    Uint16  all;
    struct  I2CEMDR_BITS  bit;
};

struct I2CPSC_BITS {                    
    Uint16 IPSC:8;                      
    Uint16 rsvd1:8;                     
};

union I2CPSC_REG {
    Uint16  all;
    struct  I2CPSC_BITS  bit;
};

struct I2CFFTX_BITS {                   
    Uint16 TXFFIL:5;                    
    Uint16 TXFFIENA:1;                  
    Uint16 TXFFINTCLR:1;                
    Uint16 TXFFINT:1;                   
    Uint16 TXFFST:5;                    
    Uint16 TXFFRST:1;                   
    Uint16 I2CFFEN:1;                   
    Uint16 rsvd1:1;                     
};

union I2CFFTX_REG {
    Uint16  all;
    struct  I2CFFTX_BITS  bit;
};

struct I2CFFRX_BITS {                   
    Uint16 RXFFIL:5;                    
    Uint16 RXFFIENA:1;                  
    Uint16 RXFFINTCLR:1;                
    Uint16 RXFFINT:1;                   
    Uint16 RXFFST:5;                    
    Uint16 RXFFRST:1;                   
    Uint16 rsvd1:2;                     
};

union I2CFFRX_REG {
    Uint16  all;
    struct  I2CFFRX_BITS  bit;
};

struct I2C_REGS {
    union   I2COAR_REG                       I2COAR;                       
    union   I2CIER_REG                       I2CIER;                       
    union   I2CSTR_REG                       I2CSTR;                       
    Uint16                                   I2CCLKL;                      
    Uint16                                   I2CCLKH;                      
    Uint16                                   I2CCNT;                       
    union   I2CDRR_REG                       I2CDRR;                       
    union   I2CSAR_REG                       I2CSAR;                       
    union   I2CDXR_REG                       I2CDXR;                       
    union   I2CMDR_REG                       I2CMDR;                       
    union   I2CISRC_REG                      I2CISRC;                      
    union   I2CEMDR_REG                      I2CEMDR;                      
    union   I2CPSC_REG                       I2CPSC;                       
    Uint16                                   rsvd1[19];                    
    union   I2CFFTX_REG                      I2CFFTX;                      
    union   I2CFFRX_REG                      I2CFFRX;                      
};




extern volatile struct I2C_REGS I2caRegs;
extern volatile struct I2C_REGS I2cbRegs;



















































struct INPUTSELECTLOCK_BITS {           
    Uint16 INPUT1SELECT:1;              
    Uint16 INPUT2SELECT:1;              
    Uint16 INPUT3SELECT:1;              
    Uint16 INPUT4SELECT:1;              
    Uint16 INPUT5SELECT:1;              
    Uint16 INPUT6SELECT:1;              
    Uint16 INPUT7SELECT:1;              
    Uint16 INPUT8SELECT:1;              
    Uint16 INPUT9SELECT:1;              
    Uint16 INPUT10SELECT:1;             
    Uint16 INPUT11SELECT:1;             
    Uint16 INPUT12SELECT:1;             
    Uint16 INPUT13SELECT:1;             
    Uint16 INPUT14SELECT:1;             
    Uint16 INPUT15SELECT:1;             
    Uint16 INPUT16SELECT:1;             
    Uint16 rsvd1:16;                    
};

union INPUTSELECTLOCK_REG {
    Uint32  all;
    struct  INPUTSELECTLOCK_BITS  bit;
};

struct INPUT_XBAR_REGS {
    Uint16                                   INPUT1SELECT;                 
    Uint16                                   INPUT2SELECT;                 
    Uint16                                   INPUT3SELECT;                 
    Uint16                                   INPUT4SELECT;                 
    Uint16                                   INPUT5SELECT;                 
    Uint16                                   INPUT6SELECT;                 
    Uint16                                   INPUT7SELECT;                 
    Uint16                                   INPUT8SELECT;                 
    Uint16                                   INPUT9SELECT;                 
    Uint16                                   INPUT10SELECT;                
    Uint16                                   INPUT11SELECT;                
    Uint16                                   INPUT12SELECT;                
    Uint16                                   INPUT13SELECT;                
    Uint16                                   INPUT14SELECT;                
    Uint16                                   INPUT15SELECT;                
    Uint16                                   INPUT16SELECT;                
    Uint16                                   rsvd1[14];                    
    union   INPUTSELECTLOCK_REG              INPUTSELECTLOCK;              
};




extern volatile struct INPUT_XBAR_REGS InputXbarRegs;
extern volatile struct INPUT_XBAR_REGS ClbInputXbarRegs;



















































struct DxLOCK_BITS {                    
    Uint16 LOCK_M0:1;                   
    Uint16 LOCK_M1:1;                   
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:12;                    
    Uint16 rsvd4:16;                    
};

union DxLOCK_REG {
    Uint32  all;
    struct  DxLOCK_BITS  bit;
};

struct DxCOMMIT_BITS {                  
    Uint16 COMMIT_M0:1;                 
    Uint16 COMMIT_M1:1;                 
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:12;                    
    Uint16 rsvd4:16;                    
};

union DxCOMMIT_REG {
    Uint32  all;
    struct  DxCOMMIT_BITS  bit;
};

struct DxACCPROT0_BITS {                
    Uint16 FETCHPROT_M0:1;              
    Uint16 CPUWRPROT_M0:1;              
    Uint16 rsvd1:6;                     
    Uint16 FETCHPROT_M1:1;              
    Uint16 CPUWRPROT_M1:1;              
    Uint16 rsvd2:6;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:6;                     
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:1;                     
    Uint16 rsvd8:6;                     
};

union DxACCPROT0_REG {
    Uint32  all;
    struct  DxACCPROT0_BITS  bit;
};

struct DxTEST_BITS {                    
    Uint16 TEST_M0:2;                   
    Uint16 TEST_M1:2;                   
    Uint16 rsvd1:2;                     
    Uint16 rsvd2:2;                     
    Uint16 rsvd3:8;                     
    Uint16 rsvd4:16;                    
};

union DxTEST_REG {
    Uint32  all;
    struct  DxTEST_BITS  bit;
};

struct DxINIT_BITS {                    
    Uint16 INIT_M0:1;                   
    Uint16 INIT_M1:1;                   
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:12;                    
    Uint16 rsvd4:16;                    
};

union DxINIT_REG {
    Uint32  all;
    struct  DxINIT_BITS  bit;
};

struct DxINITDONE_BITS {                
    Uint16 INITDONE_M0:1;               
    Uint16 INITDONE_M1:1;               
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:12;                    
    Uint16 rsvd4:16;                    
};

union DxINITDONE_REG {
    Uint32  all;
    struct  DxINITDONE_BITS  bit;
};

struct DxRAMTEST_LOCK_BITS {            
    Uint16 M0:1;                        
    Uint16 M1:1;                        
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:12;                    
    Uint16 KEY:16;                      
};

union DxRAMTEST_LOCK_REG {
    Uint32  all;
    struct  DxRAMTEST_LOCK_BITS  bit;
};

struct LSxLOCK_BITS {                   
    Uint16 LOCK_LS0:1;                  
    Uint16 LOCK_LS1:1;                  
    Uint16 LOCK_LS2:1;                  
    Uint16 LOCK_LS3:1;                  
    Uint16 LOCK_LS4:1;                  
    Uint16 LOCK_LS5:1;                  
    Uint16 LOCK_LS6:1;                  
    Uint16 LOCK_LS7:1;                  
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union LSxLOCK_REG {
    Uint32  all;
    struct  LSxLOCK_BITS  bit;
};

struct LSxCOMMIT_BITS {                 
    Uint16 COMMIT_LS0:1;                
    Uint16 COMMIT_LS1:1;                
    Uint16 COMMIT_LS2:1;                
    Uint16 COMMIT_LS3:1;                
    Uint16 COMMIT_LS4:1;                
    Uint16 COMMIT_LS5:1;                
    Uint16 COMMIT_LS6:1;                
    Uint16 COMMIT_LS7:1;                
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union LSxCOMMIT_REG {
    Uint32  all;
    struct  LSxCOMMIT_BITS  bit;
};

struct LSxMSEL_BITS {                   
    Uint16 MSEL_LS0:2;                  
    Uint16 MSEL_LS1:2;                  
    Uint16 MSEL_LS2:2;                  
    Uint16 MSEL_LS3:2;                  
    Uint16 MSEL_LS4:2;                  
    Uint16 MSEL_LS5:2;                  
    Uint16 MSEL_LS6:2;                  
    Uint16 MSEL_LS7:2;                  
    Uint16 rsvd1:16;                    
};

union LSxMSEL_REG {
    Uint32  all;
    struct  LSxMSEL_BITS  bit;
};

struct LSxCLAPGM_BITS {                 
    Uint16 CLAPGM_LS0:1;                
    Uint16 CLAPGM_LS1:1;                
    Uint16 CLAPGM_LS2:1;                
    Uint16 CLAPGM_LS3:1;                
    Uint16 CLAPGM_LS4:1;                
    Uint16 CLAPGM_LS5:1;                
    Uint16 CLAPGM_LS6:1;                
    Uint16 CLAPGM_LS7:1;                
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union LSxCLAPGM_REG {
    Uint32  all;
    struct  LSxCLAPGM_BITS  bit;
};

struct LSxACCPROT0_BITS {               
    Uint16 FETCHPROT_LS0:1;             
    Uint16 CPUWRPROT_LS0:1;             
    Uint16 rsvd1:6;                     
    Uint16 FETCHPROT_LS1:1;             
    Uint16 CPUWRPROT_LS1:1;             
    Uint16 rsvd2:6;                     
    Uint16 FETCHPROT_LS2:1;             
    Uint16 CPUWRPROT_LS2:1;             
    Uint16 rsvd3:6;                     
    Uint16 FETCHPROT_LS3:1;             
    Uint16 CPUWRPROT_LS3:1;             
    Uint16 rsvd4:6;                     
};

union LSxACCPROT0_REG {
    Uint32  all;
    struct  LSxACCPROT0_BITS  bit;
};

struct LSxACCPROT1_BITS {               
    Uint16 FETCHPROT_LS4:1;             
    Uint16 CPUWRPROT_LS4:1;             
    Uint16 rsvd1:6;                     
    Uint16 FETCHPROT_LS5:1;             
    Uint16 CPUWRPROT_LS5:1;             
    Uint16 rsvd2:6;                     
    Uint16 FETCHPROT_LS6:1;             
    Uint16 CPUWRPROT_LS6:1;             
    Uint16 rsvd3:6;                     
    Uint16 FETCHPROT_LS7:1;             
    Uint16 CPUWRPROT_LS7:1;             
    Uint16 rsvd4:6;                     
};

union LSxACCPROT1_REG {
    Uint32  all;
    struct  LSxACCPROT1_BITS  bit;
};

struct LSxTEST_BITS {                   
    Uint16 TEST_LS0:2;                  
    Uint16 TEST_LS1:2;                  
    Uint16 TEST_LS2:2;                  
    Uint16 TEST_LS3:2;                  
    Uint16 TEST_LS4:2;                  
    Uint16 TEST_LS5:2;                  
    Uint16 TEST_LS6:2;                  
    Uint16 TEST_LS7:2;                  
    Uint16 rsvd1:16;                    
};

union LSxTEST_REG {
    Uint32  all;
    struct  LSxTEST_BITS  bit;
};

struct LSxINIT_BITS {                   
    Uint16 INIT_LS0:1;                  
    Uint16 INIT_LS1:1;                  
    Uint16 INIT_LS2:1;                  
    Uint16 INIT_LS3:1;                  
    Uint16 INIT_LS4:1;                  
    Uint16 INIT_LS5:1;                  
    Uint16 INIT_LS6:1;                  
    Uint16 INIT_LS7:1;                  
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union LSxINIT_REG {
    Uint32  all;
    struct  LSxINIT_BITS  bit;
};

struct LSxINITDONE_BITS {               
    Uint16 INITDONE_LS0:1;              
    Uint16 INITDONE_LS1:1;              
    Uint16 INITDONE_LS2:1;              
    Uint16 INITDONE_LS3:1;              
    Uint16 INITDONE_LS4:1;              
    Uint16 INITDONE_LS5:1;              
    Uint16 INITDONE_LS6:1;              
    Uint16 INITDONE_LS7:1;              
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union LSxINITDONE_REG {
    Uint32  all;
    struct  LSxINITDONE_BITS  bit;
};

struct LSxRAMTEST_LOCK_BITS {           
    Uint16 LS0:1;                       
    Uint16 LS1:1;                       
    Uint16 LS2:1;                       
    Uint16 LS3:1;                       
    Uint16 LS4:1;                       
    Uint16 LS5:1;                       
    Uint16 LS6:1;                       
    Uint16 LS7:1;                       
    Uint16 rsvd1:8;                     
    Uint16 KEY:16;                      
};

union LSxRAMTEST_LOCK_REG {
    Uint32  all;
    struct  LSxRAMTEST_LOCK_BITS  bit;
};

struct GSxLOCK_BITS {                   
    Uint16 LOCK_GS0:1;                  
    Uint16 LOCK_GS1:1;                  
    Uint16 LOCK_GS2:1;                  
    Uint16 LOCK_GS3:1;                  
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:1;                     
    Uint16 rsvd8:1;                     
    Uint16 rsvd9:1;                     
    Uint16 rsvd10:1;                    
    Uint16 rsvd11:1;                    
    Uint16 rsvd12:1;                    
    Uint16 rsvd13:16;                   
};

union GSxLOCK_REG {
    Uint32  all;
    struct  GSxLOCK_BITS  bit;
};

struct GSxCOMMIT_BITS {                 
    Uint16 COMMIT_GS0:1;                
    Uint16 COMMIT_GS1:1;                
    Uint16 COMMIT_GS2:1;                
    Uint16 COMMIT_GS3:1;                
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:1;                     
    Uint16 rsvd8:1;                     
    Uint16 rsvd9:1;                     
    Uint16 rsvd10:1;                    
    Uint16 rsvd11:1;                    
    Uint16 rsvd12:1;                    
    Uint16 rsvd13:16;                   
};

union GSxCOMMIT_REG {
    Uint32  all;
    struct  GSxCOMMIT_BITS  bit;
};

struct GSxACCPROT0_BITS {               
    Uint16 FETCHPROT_GS0:1;             
    Uint16 CPUWRPROT_GS0:1;             
    Uint16 DMAWRPROT_GS0:1;             
    Uint16 HICWRPROT_GS0:1;             
    Uint16 rsvd1:4;                     
    Uint16 FETCHPROT_GS1:1;             
    Uint16 CPUWRPROT_GS1:1;             
    Uint16 DMAWRPROT_GS1:1;             
    Uint16 HICWRPROT_GS1:1;             
    Uint16 rsvd2:4;                     
    Uint16 FETCHPROT_GS2:1;             
    Uint16 CPUWRPROT_GS2:1;             
    Uint16 DMAWRPROT_GS2:1;             
    Uint16 HICWRPROT_GS2:1;             
    Uint16 rsvd3:4;                     
    Uint16 FETCHPROT_GS3:1;             
    Uint16 CPUWRPROT_GS3:1;             
    Uint16 DMAWRPROT_GS3:1;             
    Uint16 HICWRPROT_GS3:1;             
    Uint16 rsvd4:4;                     
};

union GSxACCPROT0_REG {
    Uint32  all;
    struct  GSxACCPROT0_BITS  bit;
};

struct GSxTEST_BITS {                   
    Uint16 TEST_GS0:2;                  
    Uint16 TEST_GS1:2;                  
    Uint16 TEST_GS2:2;                  
    Uint16 TEST_GS3:2;                  
    Uint16 rsvd1:2;                     
    Uint16 rsvd2:2;                     
    Uint16 rsvd3:2;                     
    Uint16 rsvd4:2;                     
    Uint16 rsvd5:2;                     
    Uint16 rsvd6:2;                     
    Uint16 rsvd7:2;                     
    Uint16 rsvd8:2;                     
    Uint16 rsvd9:2;                     
    Uint16 rsvd10:2;                    
    Uint16 rsvd11:2;                    
    Uint16 rsvd12:2;                    
};

union GSxTEST_REG {
    Uint32  all;
    struct  GSxTEST_BITS  bit;
};

struct GSxINIT_BITS {                   
    Uint16 INIT_GS0:1;                  
    Uint16 INIT_GS1:1;                  
    Uint16 INIT_GS2:1;                  
    Uint16 INIT_GS3:1;                  
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:1;                     
    Uint16 rsvd8:1;                     
    Uint16 rsvd9:1;                     
    Uint16 rsvd10:1;                    
    Uint16 rsvd11:1;                    
    Uint16 rsvd12:1;                    
    Uint16 rsvd13:16;                   
};

union GSxINIT_REG {
    Uint32  all;
    struct  GSxINIT_BITS  bit;
};

struct GSxINITDONE_BITS {               
    Uint16 INITDONE_GS0:1;              
    Uint16 INITDONE_GS1:1;              
    Uint16 INITDONE_GS2:1;              
    Uint16 INITDONE_GS3:1;              
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:1;                     
    Uint16 rsvd8:1;                     
    Uint16 rsvd9:1;                     
    Uint16 rsvd10:1;                    
    Uint16 rsvd11:1;                    
    Uint16 rsvd12:1;                    
    Uint16 rsvd13:16;                   
};

union GSxINITDONE_REG {
    Uint32  all;
    struct  GSxINITDONE_BITS  bit;
};

struct GSxRAMTEST_LOCK_BITS {           
    Uint16 GS0:1;                       
    Uint16 GS1:1;                       
    Uint16 GS2:1;                       
    Uint16 GS3:1;                       
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:1;                     
    Uint16 rsvd8:1;                     
    Uint16 rsvd9:1;                     
    Uint16 rsvd10:1;                    
    Uint16 rsvd11:1;                    
    Uint16 rsvd12:1;                    
    Uint16 KEY:16;                      
};

union GSxRAMTEST_LOCK_REG {
    Uint32  all;
    struct  GSxRAMTEST_LOCK_BITS  bit;
};

struct MSGxLOCK_BITS {                  
    Uint16 rsvd1:1;                     
    Uint16 LOCK_CPUTOCLA1:1;            
    Uint16 LOCK_CLA1TOCPU:1;            
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 LOCK_CLA1TODMA:1;            
    Uint16 LOCK_DMATOCLA1:1;            
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:1;                     
    Uint16 rsvd8:1;                     
    Uint16 rsvd9:4;                     
    Uint16 rsvd10:16;                   
};

union MSGxLOCK_REG {
    Uint32  all;
    struct  MSGxLOCK_BITS  bit;
};

struct MSGxCOMMIT_BITS {                
    Uint16 rsvd1:1;                     
    Uint16 COMMIT_CPUTOCLA1:1;          
    Uint16 COMMIT_CLA1TOCPU:1;          
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 COMMIT_CLA1TODMA:1;          
    Uint16 COMMIT_DMATOCLA1:1;          
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:1;                     
    Uint16 rsvd8:1;                     
    Uint16 rsvd9:4;                     
    Uint16 rsvd10:16;                   
};

union MSGxCOMMIT_REG {
    Uint32  all;
    struct  MSGxCOMMIT_BITS  bit;
};

struct MSGxTEST_BITS {                  
    Uint16 rsvd1:2;                     
    Uint16 TEST_CPUTOCLA1:2;            
    Uint16 TEST_CLA1TOCPU:2;            
    Uint16 rsvd2:2;                     
    Uint16 rsvd3:2;                     
    Uint16 TEST_CLA1TODMA:2;            
    Uint16 TEST_DMATOCLA1:2;            
    Uint16 rsvd4:2;                     
    Uint16 rsvd5:2;                     
    Uint16 rsvd6:2;                     
    Uint16 rsvd7:2;                     
    Uint16 rsvd8:2;                     
    Uint16 rsvd9:8;                     
};

union MSGxTEST_REG {
    Uint32  all;
    struct  MSGxTEST_BITS  bit;
};

struct MSGxINIT_BITS {                  
    Uint16 rsvd1:1;                     
    Uint16 INIT_CPUTOCLA1:1;            
    Uint16 INIT_CLA1TOCPU:1;            
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 INIT_CLA1TODMA:1;            
    Uint16 INIT_DMATOCLA1:1;            
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:1;                     
    Uint16 rsvd8:1;                     
    Uint16 rsvd9:4;                     
    Uint16 rsvd10:16;                   
};

union MSGxINIT_REG {
    Uint32  all;
    struct  MSGxINIT_BITS  bit;
};

struct MSGxINITDONE_BITS {              
    Uint16 rsvd1:1;                     
    Uint16 INITDONE_CPUTOCLA1:1;        
    Uint16 INITDONE_CLA1TOCPU:1;        
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 INITDONE_CLA1TODMA:1;        
    Uint16 INITDONE_DMATOCLA1:1;        
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:1;                     
    Uint16 rsvd8:1;                     
    Uint16 rsvd9:4;                     
    Uint16 rsvd10:16;                   
};

union MSGxINITDONE_REG {
    Uint32  all;
    struct  MSGxINITDONE_BITS  bit;
};

struct MSGxRAMTEST_LOCK_BITS {          
    Uint16 rsvd1:1;                     
    Uint16 CPUTOCLA1:1;                 
    Uint16 CLA1TOCPU:1;                 
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 CLA1TODMA:1;                 
    Uint16 DMATOCLA1:1;                 
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:1;                     
    Uint16 rsvd8:1;                     
    Uint16 rsvd9:4;                     
    Uint16 KEY:16;                      
};

union MSGxRAMTEST_LOCK_REG {
    Uint32  all;
    struct  MSGxRAMTEST_LOCK_BITS  bit;
};

struct ROM_LOCK_BITS {                  
    Uint16 LOCK_BOOTROM:1;              
    Uint16 LOCK_SECUREROM:1;            
    Uint16 LOCK_CLADATAROM:1;           
    Uint16 LOCK_CLAPROGROM:1;           
    Uint16 rsvd1:12;                    
    Uint16 KEY:16;                      
};

union ROM_LOCK_REG {
    Uint32  all;
    struct  ROM_LOCK_BITS  bit;
};

struct ROM_TEST_BITS {                  
    Uint16 TEST_BOOTROM:2;              
    Uint16 TEST_SECUREROM:2;            
    Uint16 TEST_CLADATAROM:2;           
    Uint16 TEST_CLAPROGROM:2;           
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union ROM_TEST_REG {
    Uint32  all;
    struct  ROM_TEST_BITS  bit;
};

struct ROM_FORCE_ERROR_BITS {           
    Uint16 FORCE_BOOTROM_ERROR:1;       
    Uint16 FORCE_SECUREROM_ERROR:1;     
    Uint16 FORCE_CLADATAROM_ERROR:1;    
    Uint16 FORCE_CLAPROGROM_ERROR:1;    
    Uint16 rsvd1:12;                    
    Uint16 rsvd2:16;                    
};

union ROM_FORCE_ERROR_REG {
    Uint32  all;
    struct  ROM_FORCE_ERROR_BITS  bit;
};

struct MEM_CFG_REGS {
    union   DxLOCK_REG                       DxLOCK;                       
    union   DxCOMMIT_REG                     DxCOMMIT;                     
    Uint16                                   rsvd1[4];                     
    union   DxACCPROT0_REG                   DxACCPROT0;                   
    Uint16                                   rsvd2[6];                     
    union   DxTEST_REG                       DxTEST;                       
    union   DxINIT_REG                       DxINIT;                       
    union   DxINITDONE_REG                   DxINITDONE;                   
    union   DxRAMTEST_LOCK_REG               DxRAMTEST_LOCK;               
    Uint16                                   rsvd3[8];                     
    union   LSxLOCK_REG                      LSxLOCK;                      
    union   LSxCOMMIT_REG                    LSxCOMMIT;                    
    union   LSxMSEL_REG                      LSxMSEL;                      
    union   LSxCLAPGM_REG                    LSxCLAPGM;                    
    union   LSxACCPROT0_REG                  LSxACCPROT0;                  
    union   LSxACCPROT1_REG                  LSxACCPROT1;                  
    Uint16                                   rsvd4[4];                     
    union   LSxTEST_REG                      LSxTEST;                      
    union   LSxINIT_REG                      LSxINIT;                      
    union   LSxINITDONE_REG                  LSxINITDONE;                  
    union   LSxRAMTEST_LOCK_REG              LSxRAMTEST_LOCK;              
    Uint16                                   rsvd5[8];                     
    union   GSxLOCK_REG                      GSxLOCK;                      
    union   GSxCOMMIT_REG                    GSxCOMMIT;                    
    Uint16                                   rsvd6[4];                     
    union   GSxACCPROT0_REG                  GSxACCPROT0;                  
    Uint16                                   rsvd7[6];                     
    union   GSxTEST_REG                      GSxTEST;                      
    union   GSxINIT_REG                      GSxINIT;                      
    union   GSxINITDONE_REG                  GSxINITDONE;                  
    union   GSxRAMTEST_LOCK_REG              GSxRAMTEST_LOCK;              
    Uint16                                   rsvd8[8];                     
    union   MSGxLOCK_REG                     MSGxLOCK;                     
    union   MSGxCOMMIT_REG                   MSGxCOMMIT;                   
    Uint16                                   rsvd9[12];                    
    union   MSGxTEST_REG                     MSGxTEST;                     
    union   MSGxINIT_REG                     MSGxINIT;                     
    union   MSGxINITDONE_REG                 MSGxINITDONE;                 
    union   MSGxRAMTEST_LOCK_REG             MSGxRAMTEST_LOCK;             
    Uint16                                   rsvd10[40];                   
    union   ROM_LOCK_REG                     ROM_LOCK;                     
    union   ROM_TEST_REG                     ROM_TEST;                     
    union   ROM_FORCE_ERROR_REG              ROM_FORCE_ERROR;              
};

struct NMAVFLG_BITS {                   
    Uint16 CPUREAD:1;                   
    Uint16 CPUWRITE:1;                  
    Uint16 CPUFETCH:1;                  
    Uint16 DMAWRITE:1;                  
    Uint16 CLA1READ:1;                  
    Uint16 CLA1WRITE:1;                 
    Uint16 CLA1FETCH:1;                 
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 DMAREAD:1;                   
    Uint16 HICREAD:1;                   
    Uint16 HICWRITE:1;                  
    Uint16 rsvd4:3;                     
    Uint16 rsvd5:16;                    
};

union NMAVFLG_REG {
    Uint32  all;
    struct  NMAVFLG_BITS  bit;
};

struct NMAVSET_BITS {                   
    Uint16 CPUREAD:1;                   
    Uint16 CPUWRITE:1;                  
    Uint16 CPUFETCH:1;                  
    Uint16 DMAWRITE:1;                  
    Uint16 CLA1READ:1;                  
    Uint16 CLA1WRITE:1;                 
    Uint16 CLA1FETCH:1;                 
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 DMAREAD:1;                   
    Uint16 HICREAD:1;                   
    Uint16 HICWRITE:1;                  
    Uint16 rsvd4:3;                     
    Uint16 rsvd5:16;                    
};

union NMAVSET_REG {
    Uint32  all;
    struct  NMAVSET_BITS  bit;
};

struct NMAVCLR_BITS {                   
    Uint16 CPUREAD:1;                   
    Uint16 CPUWRITE:1;                  
    Uint16 CPUFETCH:1;                  
    Uint16 DMAWRITE:1;                  
    Uint16 CLA1READ:1;                  
    Uint16 CLA1WRITE:1;                 
    Uint16 CLA1FETCH:1;                 
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 DMAREAD:1;                   
    Uint16 HICREAD:1;                   
    Uint16 HICWRITE:1;                  
    Uint16 rsvd4:3;                     
    Uint16 rsvd5:16;                    
};

union NMAVCLR_REG {
    Uint32  all;
    struct  NMAVCLR_BITS  bit;
};

struct NMAVINTEN_BITS {                 
    Uint16 CPUREAD:1;                   
    Uint16 CPUWRITE:1;                  
    Uint16 CPUFETCH:1;                  
    Uint16 DMAWRITE:1;                  
    Uint16 CLA1READ:1;                  
    Uint16 CLA1WRITE:1;                 
    Uint16 CLA1FETCH:1;                 
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 DMAREAD:1;                   
    Uint16 HICREAD:1;                   
    Uint16 HICWRITE:1;                  
    Uint16 rsvd4:3;                     
    Uint16 rsvd5:16;                    
};

union NMAVINTEN_REG {
    Uint32  all;
    struct  NMAVINTEN_BITS  bit;
};

struct MAVFLG_BITS {                    
    Uint16 CPUFETCH:1;                  
    Uint16 CPUWRITE:1;                  
    Uint16 DMAWRITE:1;                  
    Uint16 HICAWRITE:1;                 
    Uint16 rsvd1:12;                    
    Uint16 rsvd2:16;                    
};

union MAVFLG_REG {
    Uint32  all;
    struct  MAVFLG_BITS  bit;
};

struct MAVSET_BITS {                    
    Uint16 CPUFETCH:1;                  
    Uint16 CPUWRITE:1;                  
    Uint16 DMAWRITE:1;                  
    Uint16 HICAWRITE:1;                 
    Uint16 rsvd1:12;                    
    Uint16 rsvd2:16;                    
};

union MAVSET_REG {
    Uint32  all;
    struct  MAVSET_BITS  bit;
};

struct MAVCLR_BITS {                    
    Uint16 CPUFETCH:1;                  
    Uint16 CPUWRITE:1;                  
    Uint16 DMAWRITE:1;                  
    Uint16 HICAWRITE:1;                 
    Uint16 rsvd1:12;                    
    Uint16 rsvd2:16;                    
};

union MAVCLR_REG {
    Uint32  all;
    struct  MAVCLR_BITS  bit;
};

struct MAVINTEN_BITS {                  
    Uint16 CPUFETCH:1;                  
    Uint16 CPUWRITE:1;                  
    Uint16 DMAWRITE:1;                  
    Uint16 HICAWRITE:1;                 
    Uint16 rsvd1:12;                    
    Uint16 rsvd2:16;                    
};

union MAVINTEN_REG {
    Uint32  all;
    struct  MAVINTEN_BITS  bit;
};

struct ACCESS_PROTECTION_REGS {
    union   NMAVFLG_REG                      NMAVFLG;                      
    union   NMAVSET_REG                      NMAVSET;                      
    union   NMAVCLR_REG                      NMAVCLR;                      
    union   NMAVINTEN_REG                    NMAVINTEN;                    
    Uint32                                   NMCPURDAVADDR;                
    Uint32                                   NMCPUWRAVADDR;                
    Uint32                                   NMCPUFAVADDR;                 
    Uint32                                   NMDMAWRAVADDR;                
    Uint32                                   NMCLA1RDAVADDR;               
    Uint32                                   NMCLA1WRAVADDR;               
    Uint32                                   NMCLA1FAVADDR;                
    Uint16                                   rsvd1[6];                     
    Uint32                                   NMDMARDAVADDR;                
    Uint16                                   rsvd2[2];                     
    union   MAVFLG_REG                       MAVFLG;                       
    union   MAVSET_REG                       MAVSET;                       
    union   MAVCLR_REG                       MAVCLR;                       
    union   MAVINTEN_REG                     MAVINTEN;                     
    Uint32                                   MCPUFAVADDR;                  
    Uint32                                   MCPUWRAVADDR;                 
    Uint32                                   MDMAWRAVADDR;                 
    Uint32                                   MHICWRAVADDR[2];              
    Uint16                                   rsvd3[10];                    
    Uint32                                   NMHICRDAVADDR;                
    Uint32                                   NMHICWRAVADDR;                
};

struct UCERRFLG_BITS {                  
    Uint16 CPURDERR:1;                  
    Uint16 DMARDERR:1;                  
    Uint16 CLA1RDERR:1;                 
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 HICARDERR:1;                 
    Uint16 rsvd3:10;                    
    Uint16 rsvd4:16;                    
};

union UCERRFLG_REG {
    Uint32  all;
    struct  UCERRFLG_BITS  bit;
};

struct UCERRSET_BITS {                  
    Uint16 CPURDERR:1;                  
    Uint16 DMARDERR:1;                  
    Uint16 CLA1RDERR:1;                 
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 HICARDERR:1;                 
    Uint16 rsvd3:10;                    
    Uint16 rsvd4:16;                    
};

union UCERRSET_REG {
    Uint32  all;
    struct  UCERRSET_BITS  bit;
};

struct UCERRCLR_BITS {                  
    Uint16 CPURDERR:1;                  
    Uint16 DMARDERR:1;                  
    Uint16 CLA1RDERR:1;                 
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 HICARDERR:1;                 
    Uint16 rsvd3:10;                    
    Uint16 rsvd4:16;                    
};

union UCERRCLR_REG {
    Uint32  all;
    struct  UCERRCLR_BITS  bit;
};

struct CERRFLG_BITS {                   
    Uint16 CPURDERR:1;                  
    Uint16 DMARDERR:1;                  
    Uint16 CLA1RDERR:1;                 
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 HICRDERR:1;                  
    Uint16 rsvd3:10;                    
    Uint16 rsvd4:16;                    
};

union CERRFLG_REG {
    Uint32  all;
    struct  CERRFLG_BITS  bit;
};

struct CERRSET_BITS {                   
    Uint16 CPURDERR:1;                  
    Uint16 DMARDERR:1;                  
    Uint16 CLA1RDERR:1;                 
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 HICRDERR:1;                  
    Uint16 rsvd3:10;                    
    Uint16 rsvd4:16;                    
};

union CERRSET_REG {
    Uint32  all;
    struct  CERRSET_BITS  bit;
};

struct CERRCLR_BITS {                   
    Uint16 CPURDERR:1;                  
    Uint16 DMARDERR:1;                  
    Uint16 CLA1RDERR:1;                 
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 HICRDERR:1;                  
    Uint16 rsvd3:10;                    
    Uint16 rsvd4:16;                    
};

union CERRCLR_REG {
    Uint32  all;
    struct  CERRCLR_BITS  bit;
};

struct CERRTHRES_BITS {                 
    Uint16 CERRTHRES:16;                
    Uint16 rsvd1:16;                    
};

union CERRTHRES_REG {
    Uint32  all;
    struct  CERRTHRES_BITS  bit;
};

struct CEINTFLG_BITS {                  
    Uint16 CEINTFLAG:1;                 
    Uint16 rsvd1:15;                    
    Uint16 rsvd2:16;                    
};

union CEINTFLG_REG {
    Uint32  all;
    struct  CEINTFLG_BITS  bit;
};

struct CEINTCLR_BITS {                  
    Uint16 CEINTCLR:1;                  
    Uint16 rsvd1:15;                    
    Uint16 rsvd2:16;                    
};

union CEINTCLR_REG {
    Uint32  all;
    struct  CEINTCLR_BITS  bit;
};

struct CEINTSET_BITS {                  
    Uint16 CEINTSET:1;                  
    Uint16 rsvd1:15;                    
    Uint16 rsvd2:16;                    
};

union CEINTSET_REG {
    Uint32  all;
    struct  CEINTSET_BITS  bit;
};

struct CEINTEN_BITS {                   
    Uint16 CEINTEN:1;                   
    Uint16 rsvd1:15;                    
    Uint16 rsvd2:16;                    
};

union CEINTEN_REG {
    Uint32  all;
    struct  CEINTEN_BITS  bit;
};

struct MEMORY_ERROR_REGS {
    union   UCERRFLG_REG                     UCERRFLG;                     
    union   UCERRSET_REG                     UCERRSET;                     
    union   UCERRCLR_REG                     UCERRCLR;                     
    Uint32                                   UCCPUREADDR;                  
    Uint32                                   UCDMAREADDR;                  
    Uint32                                   UCCLA1READDR;                 
    Uint16                                   rsvd1[4];                     
    Uint32                                   UCHICAREADDR;                 
    Uint16                                   rsvd2[14];                    
    union   CERRFLG_REG                      CERRFLG;                      
    union   CERRSET_REG                      CERRSET;                      
    union   CERRCLR_REG                      CERRCLR;                      
    Uint32                                   CCPUREADDR;                   
    Uint32                                   CDMAREADDR;                   
    Uint32                                   CCLA1READDR;                  
    Uint16                                   rsvd3[2];                     
    Uint32                                   CERRCNT;                      
    union   CERRTHRES_REG                    CERRTHRES;                    
    union   CEINTFLG_REG                     CEINTFLG;                     
    union   CEINTCLR_REG                     CEINTCLR;                     
    union   CEINTSET_REG                     CEINTSET;                     
    union   CEINTEN_REG                      CEINTEN;                      
    Uint32                                   CHICREADDR;                   
};

struct CPU_RAM_TEST_ERROR_STS_BITS {    
    Uint16 COR_ERROR:1;                 
    Uint16 UNC_ERROR:1;                 
    Uint16 rsvd1:14;                    
    Uint16 rsvd2:16;                    
};

union CPU_RAM_TEST_ERROR_STS_REG {
    Uint32  all;
    struct  CPU_RAM_TEST_ERROR_STS_BITS  bit;
};

struct CPU_RAM_TEST_ERROR_STS_CLR_BITS {
    Uint16 COR_ERROR:1;                 
    Uint16 UNC_ERROR:1;                 
    Uint16 rsvd1:14;                    
    Uint16 rsvd2:16;                    
};

union CPU_RAM_TEST_ERROR_STS_CLR_REG {
    Uint32  all;
    struct  CPU_RAM_TEST_ERROR_STS_CLR_BITS  bit;
};

struct TEST_ERROR_REGS {
    union   CPU_RAM_TEST_ERROR_STS_REG       CPU_RAM_TEST_ERROR_STS;       
    union   CPU_RAM_TEST_ERROR_STS_CLR_REG   CPU_RAM_TEST_ERROR_STS_CLR;   
    Uint32                                   CPU_RAM_TEST_ERROR_ADDR;      
};




extern volatile struct MEM_CFG_REGS MemCfgRegs;
extern volatile struct ACCESS_PROTECTION_REGS AccessProtectionRegs;
extern volatile struct MEMORY_ERROR_REGS MemoryErrorRegs;
extern volatile struct TEST_ERROR_REGS TestErrorRegs;



















































struct NMICFG_BITS {                    
    Uint16 NMIE:1;                      
    Uint16 rsvd1:15;                    
};

union NMICFG_REG {
    Uint16  all;
    struct  NMICFG_BITS  bit;
};

struct NMIFLG_BITS {                    
    Uint16 NMIINT:1;                    
    Uint16 CLOCKFAIL:1;                 
    Uint16 RAMUNCERR:1;                 
    Uint16 FLUNCERR:1;                  
    Uint16 CPU1HWBISTERR:1;             
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 SYSDBGNMI:1;                 
    Uint16 CLBNMI:1;                    
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:1;                     
    Uint16 SWERR:1;                     
    Uint16 CRC_FAIL:1;                  
    Uint16 rsvd7:1;                     
};

union NMIFLG_REG {
    Uint16  all;
    struct  NMIFLG_BITS  bit;
};

struct NMIFLGCLR_BITS {                 
    Uint16 NMIINT:1;                    
    Uint16 CLOCKFAIL:1;                 
    Uint16 RAMUNCERR:1;                 
    Uint16 FLUNCERR:1;                  
    Uint16 CPU1HWBISTERR:1;             
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 SYSDBGNMI:1;                 
    Uint16 RLNMI:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:1;                     
    Uint16 SWERR:1;                     
    Uint16 CRC_FAIL:1;                  
    Uint16 rsvd7:1;                     
};

union NMIFLGCLR_REG {
    Uint16  all;
    struct  NMIFLGCLR_BITS  bit;
};

struct NMIFLGFRC_BITS {                 
    Uint16 rsvd1:1;                     
    Uint16 CLOCKFAIL:1;                 
    Uint16 RAMUNCERR:1;                 
    Uint16 FLUNCERR:1;                  
    Uint16 CPU1HWBISTERR:1;             
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 SYSDBGNMI:1;                 
    Uint16 RLNMI:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:1;                     
    Uint16 SWERR:1;                     
    Uint16 CRC_FAIL:1;                  
    Uint16 rsvd8:1;                     
};

union NMIFLGFRC_REG {
    Uint16  all;
    struct  NMIFLGFRC_BITS  bit;
};

struct NMISHDFLG_BITS {                 
    Uint16 rsvd1:1;                     
    Uint16 CLOCKFAIL:1;                 
    Uint16 RAMUNCERR:1;                 
    Uint16 FLUNCERR:1;                  
    Uint16 CPU1HWBISTERR:1;             
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 SYSDBGNMI:1;                 
    Uint16 RLNMI:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:1;                     
    Uint16 SWERR:1;                     
    Uint16 CRC_FAIL:1;                  
    Uint16 rsvd8:1;                     
};

union NMISHDFLG_REG {
    Uint16  all;
    struct  NMISHDFLG_BITS  bit;
};

struct ERRORSTS_BITS {                  
    Uint16 ERROR:1;                     
    Uint16 PINSTS:1;                    
    Uint16 rsvd1:14;                    
};

union ERRORSTS_REG {
    Uint16  all;
    struct  ERRORSTS_BITS  bit;
};

struct ERRORSTSCLR_BITS {               
    Uint16 ERROR:1;                     
    Uint16 rsvd1:15;                    
};

union ERRORSTSCLR_REG {
    Uint16  all;
    struct  ERRORSTSCLR_BITS  bit;
};

struct ERRORSTSFRC_BITS {               
    Uint16 ERROR:1;                     
    Uint16 rsvd1:15;                    
};

union ERRORSTSFRC_REG {
    Uint16  all;
    struct  ERRORSTSFRC_BITS  bit;
};

struct ERRORCTL_BITS {                  
    Uint16 ERRORPOLSEL:1;               
    Uint16 rsvd1:15;                    
};

union ERRORCTL_REG {
    Uint16  all;
    struct  ERRORCTL_BITS  bit;
};

struct ERRORLOCK_BITS {                 
    Uint16 ERRORCTL:1;                  
    Uint16 rsvd1:15;                    
};

union ERRORLOCK_REG {
    Uint16  all;
    struct  ERRORLOCK_BITS  bit;
};

struct NMI_INTRUPT_REGS {
    union   NMICFG_REG                       NMICFG;                       
    union   NMIFLG_REG                       NMIFLG;                       
    union   NMIFLGCLR_REG                    NMIFLGCLR;                    
    union   NMIFLGFRC_REG                    NMIFLGFRC;                    
    Uint16                                   NMIWDCNT;                     
    Uint16                                   NMIWDPRD;                     
    union   NMISHDFLG_REG                    NMISHDFLG;                    
    union   ERRORSTS_REG                     ERRORSTS;                     
    union   ERRORSTSCLR_REG                  ERRORSTSCLR;                  
    union   ERRORSTSFRC_REG                  ERRORSTSFRC;                  
    union   ERRORCTL_REG                     ERRORCTL;                     
    union   ERRORLOCK_REG                    ERRORLOCK;                    
};




extern volatile struct NMI_INTRUPT_REGS NmiIntruptRegs;



















































struct OUTPUT1MUX0TO15CFG_BITS {        
    Uint16 MUX0:2;                      
    Uint16 MUX1:2;                      
    Uint16 MUX2:2;                      
    Uint16 MUX3:2;                      
    Uint16 MUX4:2;                      
    Uint16 MUX5:2;                      
    Uint16 MUX6:2;                      
    Uint16 MUX7:2;                      
    Uint16 MUX8:2;                      
    Uint16 MUX9:2;                      
    Uint16 MUX10:2;                     
    Uint16 MUX11:2;                     
    Uint16 MUX12:2;                     
    Uint16 MUX13:2;                     
    Uint16 MUX14:2;                     
    Uint16 MUX15:2;                     
};

union OUTPUT1MUX0TO15CFG_REG {
    Uint32  all;
    struct  OUTPUT1MUX0TO15CFG_BITS  bit;
};

struct OUTPUT1MUX16TO31CFG_BITS {       
    Uint16 MUX16:2;                     
    Uint16 MUX17:2;                     
    Uint16 MUX18:2;                     
    Uint16 MUX19:2;                     
    Uint16 MUX20:2;                     
    Uint16 MUX21:2;                     
    Uint16 MUX22:2;                     
    Uint16 MUX23:2;                     
    Uint16 MUX24:2;                     
    Uint16 MUX25:2;                     
    Uint16 MUX26:2;                     
    Uint16 MUX27:2;                     
    Uint16 MUX28:2;                     
    Uint16 MUX29:2;                     
    Uint16 MUX30:2;                     
    Uint16 MUX31:2;                     
};

union OUTPUT1MUX16TO31CFG_REG {
    Uint32  all;
    struct  OUTPUT1MUX16TO31CFG_BITS  bit;
};

struct OUTPUT2MUX0TO15CFG_BITS {        
    Uint16 MUX0:2;                      
    Uint16 MUX1:2;                      
    Uint16 MUX2:2;                      
    Uint16 MUX3:2;                      
    Uint16 MUX4:2;                      
    Uint16 MUX5:2;                      
    Uint16 MUX6:2;                      
    Uint16 MUX7:2;                      
    Uint16 MUX8:2;                      
    Uint16 MUX9:2;                      
    Uint16 MUX10:2;                     
    Uint16 MUX11:2;                     
    Uint16 MUX12:2;                     
    Uint16 MUX13:2;                     
    Uint16 MUX14:2;                     
    Uint16 MUX15:2;                     
};

union OUTPUT2MUX0TO15CFG_REG {
    Uint32  all;
    struct  OUTPUT2MUX0TO15CFG_BITS  bit;
};

struct OUTPUT2MUX16TO31CFG_BITS {       
    Uint16 MUX16:2;                     
    Uint16 MUX17:2;                     
    Uint16 MUX18:2;                     
    Uint16 MUX19:2;                     
    Uint16 MUX20:2;                     
    Uint16 MUX21:2;                     
    Uint16 MUX22:2;                     
    Uint16 MUX23:2;                     
    Uint16 MUX24:2;                     
    Uint16 MUX25:2;                     
    Uint16 MUX26:2;                     
    Uint16 MUX27:2;                     
    Uint16 MUX28:2;                     
    Uint16 MUX29:2;                     
    Uint16 MUX30:2;                     
    Uint16 MUX31:2;                     
};

union OUTPUT2MUX16TO31CFG_REG {
    Uint32  all;
    struct  OUTPUT2MUX16TO31CFG_BITS  bit;
};

struct OUTPUT3MUX0TO15CFG_BITS {        
    Uint16 MUX0:2;                      
    Uint16 MUX1:2;                      
    Uint16 MUX2:2;                      
    Uint16 MUX3:2;                      
    Uint16 MUX4:2;                      
    Uint16 MUX5:2;                      
    Uint16 MUX6:2;                      
    Uint16 MUX7:2;                      
    Uint16 MUX8:2;                      
    Uint16 MUX9:2;                      
    Uint16 MUX10:2;                     
    Uint16 MUX11:2;                     
    Uint16 MUX12:2;                     
    Uint16 MUX13:2;                     
    Uint16 MUX14:2;                     
    Uint16 MUX15:2;                     
};

union OUTPUT3MUX0TO15CFG_REG {
    Uint32  all;
    struct  OUTPUT3MUX0TO15CFG_BITS  bit;
};

struct OUTPUT3MUX16TO31CFG_BITS {       
    Uint16 MUX16:2;                     
    Uint16 MUX17:2;                     
    Uint16 MUX18:2;                     
    Uint16 MUX19:2;                     
    Uint16 MUX20:2;                     
    Uint16 MUX21:2;                     
    Uint16 MUX22:2;                     
    Uint16 MUX23:2;                     
    Uint16 MUX24:2;                     
    Uint16 MUX25:2;                     
    Uint16 MUX26:2;                     
    Uint16 MUX27:2;                     
    Uint16 MUX28:2;                     
    Uint16 MUX29:2;                     
    Uint16 MUX30:2;                     
    Uint16 MUX31:2;                     
};

union OUTPUT3MUX16TO31CFG_REG {
    Uint32  all;
    struct  OUTPUT3MUX16TO31CFG_BITS  bit;
};

struct OUTPUT4MUX0TO15CFG_BITS {        
    Uint16 MUX0:2;                      
    Uint16 MUX1:2;                      
    Uint16 MUX2:2;                      
    Uint16 MUX3:2;                      
    Uint16 MUX4:2;                      
    Uint16 MUX5:2;                      
    Uint16 MUX6:2;                      
    Uint16 MUX7:2;                      
    Uint16 MUX8:2;                      
    Uint16 MUX9:2;                      
    Uint16 MUX10:2;                     
    Uint16 MUX11:2;                     
    Uint16 MUX12:2;                     
    Uint16 MUX13:2;                     
    Uint16 MUX14:2;                     
    Uint16 MUX15:2;                     
};

union OUTPUT4MUX0TO15CFG_REG {
    Uint32  all;
    struct  OUTPUT4MUX0TO15CFG_BITS  bit;
};

struct OUTPUT4MUX16TO31CFG_BITS {       
    Uint16 MUX16:2;                     
    Uint16 MUX17:2;                     
    Uint16 MUX18:2;                     
    Uint16 MUX19:2;                     
    Uint16 MUX20:2;                     
    Uint16 MUX21:2;                     
    Uint16 MUX22:2;                     
    Uint16 MUX23:2;                     
    Uint16 MUX24:2;                     
    Uint16 MUX25:2;                     
    Uint16 MUX26:2;                     
    Uint16 MUX27:2;                     
    Uint16 MUX28:2;                     
    Uint16 MUX29:2;                     
    Uint16 MUX30:2;                     
    Uint16 MUX31:2;                     
};

union OUTPUT4MUX16TO31CFG_REG {
    Uint32  all;
    struct  OUTPUT4MUX16TO31CFG_BITS  bit;
};

struct OUTPUT5MUX0TO15CFG_BITS {        
    Uint16 MUX0:2;                      
    Uint16 MUX1:2;                      
    Uint16 MUX2:2;                      
    Uint16 MUX3:2;                      
    Uint16 MUX4:2;                      
    Uint16 MUX5:2;                      
    Uint16 MUX6:2;                      
    Uint16 MUX7:2;                      
    Uint16 MUX8:2;                      
    Uint16 MUX9:2;                      
    Uint16 MUX10:2;                     
    Uint16 MUX11:2;                     
    Uint16 MUX12:2;                     
    Uint16 MUX13:2;                     
    Uint16 MUX14:2;                     
    Uint16 MUX15:2;                     
};

union OUTPUT5MUX0TO15CFG_REG {
    Uint32  all;
    struct  OUTPUT5MUX0TO15CFG_BITS  bit;
};

struct OUTPUT5MUX16TO31CFG_BITS {       
    Uint16 MUX16:2;                     
    Uint16 MUX17:2;                     
    Uint16 MUX18:2;                     
    Uint16 MUX19:2;                     
    Uint16 MUX20:2;                     
    Uint16 MUX21:2;                     
    Uint16 MUX22:2;                     
    Uint16 MUX23:2;                     
    Uint16 MUX24:2;                     
    Uint16 MUX25:2;                     
    Uint16 MUX26:2;                     
    Uint16 MUX27:2;                     
    Uint16 MUX28:2;                     
    Uint16 MUX29:2;                     
    Uint16 MUX30:2;                     
    Uint16 MUX31:2;                     
};

union OUTPUT5MUX16TO31CFG_REG {
    Uint32  all;
    struct  OUTPUT5MUX16TO31CFG_BITS  bit;
};

struct OUTPUT6MUX0TO15CFG_BITS {        
    Uint16 MUX0:2;                      
    Uint16 MUX1:2;                      
    Uint16 MUX2:2;                      
    Uint16 MUX3:2;                      
    Uint16 MUX4:2;                      
    Uint16 MUX5:2;                      
    Uint16 MUX6:2;                      
    Uint16 MUX7:2;                      
    Uint16 MUX8:2;                      
    Uint16 MUX9:2;                      
    Uint16 MUX10:2;                     
    Uint16 MUX11:2;                     
    Uint16 MUX12:2;                     
    Uint16 MUX13:2;                     
    Uint16 MUX14:2;                     
    Uint16 MUX15:2;                     
};

union OUTPUT6MUX0TO15CFG_REG {
    Uint32  all;
    struct  OUTPUT6MUX0TO15CFG_BITS  bit;
};

struct OUTPUT6MUX16TO31CFG_BITS {       
    Uint16 MUX16:2;                     
    Uint16 MUX17:2;                     
    Uint16 MUX18:2;                     
    Uint16 MUX19:2;                     
    Uint16 MUX20:2;                     
    Uint16 MUX21:2;                     
    Uint16 MUX22:2;                     
    Uint16 MUX23:2;                     
    Uint16 MUX24:2;                     
    Uint16 MUX25:2;                     
    Uint16 MUX26:2;                     
    Uint16 MUX27:2;                     
    Uint16 MUX28:2;                     
    Uint16 MUX29:2;                     
    Uint16 MUX30:2;                     
    Uint16 MUX31:2;                     
};

union OUTPUT6MUX16TO31CFG_REG {
    Uint32  all;
    struct  OUTPUT6MUX16TO31CFG_BITS  bit;
};

struct OUTPUT7MUX0TO15CFG_BITS {        
    Uint16 MUX0:2;                      
    Uint16 MUX1:2;                      
    Uint16 MUX2:2;                      
    Uint16 MUX3:2;                      
    Uint16 MUX4:2;                      
    Uint16 MUX5:2;                      
    Uint16 MUX6:2;                      
    Uint16 MUX7:2;                      
    Uint16 MUX8:2;                      
    Uint16 MUX9:2;                      
    Uint16 MUX10:2;                     
    Uint16 MUX11:2;                     
    Uint16 MUX12:2;                     
    Uint16 MUX13:2;                     
    Uint16 MUX14:2;                     
    Uint16 MUX15:2;                     
};

union OUTPUT7MUX0TO15CFG_REG {
    Uint32  all;
    struct  OUTPUT7MUX0TO15CFG_BITS  bit;
};

struct OUTPUT7MUX16TO31CFG_BITS {       
    Uint16 MUX16:2;                     
    Uint16 MUX17:2;                     
    Uint16 MUX18:2;                     
    Uint16 MUX19:2;                     
    Uint16 MUX20:2;                     
    Uint16 MUX21:2;                     
    Uint16 MUX22:2;                     
    Uint16 MUX23:2;                     
    Uint16 MUX24:2;                     
    Uint16 MUX25:2;                     
    Uint16 MUX26:2;                     
    Uint16 MUX27:2;                     
    Uint16 MUX28:2;                     
    Uint16 MUX29:2;                     
    Uint16 MUX30:2;                     
    Uint16 MUX31:2;                     
};

union OUTPUT7MUX16TO31CFG_REG {
    Uint32  all;
    struct  OUTPUT7MUX16TO31CFG_BITS  bit;
};

struct OUTPUT8MUX0TO15CFG_BITS {        
    Uint16 MUX0:2;                      
    Uint16 MUX1:2;                      
    Uint16 MUX2:2;                      
    Uint16 MUX3:2;                      
    Uint16 MUX4:2;                      
    Uint16 MUX5:2;                      
    Uint16 MUX6:2;                      
    Uint16 MUX7:2;                      
    Uint16 MUX8:2;                      
    Uint16 MUX9:2;                      
    Uint16 MUX10:2;                     
    Uint16 MUX11:2;                     
    Uint16 MUX12:2;                     
    Uint16 MUX13:2;                     
    Uint16 MUX14:2;                     
    Uint16 MUX15:2;                     
};

union OUTPUT8MUX0TO15CFG_REG {
    Uint32  all;
    struct  OUTPUT8MUX0TO15CFG_BITS  bit;
};

struct OUTPUT8MUX16TO31CFG_BITS {       
    Uint16 MUX16:2;                     
    Uint16 MUX17:2;                     
    Uint16 MUX18:2;                     
    Uint16 MUX19:2;                     
    Uint16 MUX20:2;                     
    Uint16 MUX21:2;                     
    Uint16 MUX22:2;                     
    Uint16 MUX23:2;                     
    Uint16 MUX24:2;                     
    Uint16 MUX25:2;                     
    Uint16 MUX26:2;                     
    Uint16 MUX27:2;                     
    Uint16 MUX28:2;                     
    Uint16 MUX29:2;                     
    Uint16 MUX30:2;                     
    Uint16 MUX31:2;                     
};

union OUTPUT8MUX16TO31CFG_REG {
    Uint32  all;
    struct  OUTPUT8MUX16TO31CFG_BITS  bit;
};

struct OUTPUT1MUXENABLE_BITS {          
    Uint16 MUX0:1;                      
    Uint16 MUX1:1;                      
    Uint16 MUX2:1;                      
    Uint16 MUX3:1;                      
    Uint16 MUX4:1;                      
    Uint16 MUX5:1;                      
    Uint16 MUX6:1;                      
    Uint16 MUX7:1;                      
    Uint16 MUX8:1;                      
    Uint16 MUX9:1;                      
    Uint16 MUX10:1;                     
    Uint16 MUX11:1;                     
    Uint16 MUX12:1;                     
    Uint16 MUX13:1;                     
    Uint16 MUX14:1;                     
    Uint16 MUX15:1;                     
    Uint16 MUX16:1;                     
    Uint16 MUX17:1;                     
    Uint16 MUX18:1;                     
    Uint16 MUX19:1;                     
    Uint16 MUX20:1;                     
    Uint16 MUX21:1;                     
    Uint16 MUX22:1;                     
    Uint16 MUX23:1;                     
    Uint16 MUX24:1;                     
    Uint16 MUX25:1;                     
    Uint16 MUX26:1;                     
    Uint16 MUX27:1;                     
    Uint16 MUX28:1;                     
    Uint16 MUX29:1;                     
    Uint16 MUX30:1;                     
    Uint16 MUX31:1;                     
};

union OUTPUT1MUXENABLE_REG {
    Uint32  all;
    struct  OUTPUT1MUXENABLE_BITS  bit;
};

struct OUTPUT2MUXENABLE_BITS {          
    Uint16 MUX0:1;                      
    Uint16 MUX1:1;                      
    Uint16 MUX2:1;                      
    Uint16 MUX3:1;                      
    Uint16 MUX4:1;                      
    Uint16 MUX5:1;                      
    Uint16 MUX6:1;                      
    Uint16 MUX7:1;                      
    Uint16 MUX8:1;                      
    Uint16 MUX9:1;                      
    Uint16 MUX10:1;                     
    Uint16 MUX11:1;                     
    Uint16 MUX12:1;                     
    Uint16 MUX13:1;                     
    Uint16 MUX14:1;                     
    Uint16 MUX15:1;                     
    Uint16 MUX16:1;                     
    Uint16 MUX17:1;                     
    Uint16 MUX18:1;                     
    Uint16 MUX19:1;                     
    Uint16 MUX20:1;                     
    Uint16 MUX21:1;                     
    Uint16 MUX22:1;                     
    Uint16 MUX23:1;                     
    Uint16 MUX24:1;                     
    Uint16 MUX25:1;                     
    Uint16 MUX26:1;                     
    Uint16 MUX27:1;                     
    Uint16 MUX28:1;                     
    Uint16 MUX29:1;                     
    Uint16 MUX30:1;                     
    Uint16 MUX31:1;                     
};

union OUTPUT2MUXENABLE_REG {
    Uint32  all;
    struct  OUTPUT2MUXENABLE_BITS  bit;
};

struct OUTPUT3MUXENABLE_BITS {          
    Uint16 MUX0:1;                      
    Uint16 MUX1:1;                      
    Uint16 MUX2:1;                      
    Uint16 MUX3:1;                      
    Uint16 MUX4:1;                      
    Uint16 MUX5:1;                      
    Uint16 MUX6:1;                      
    Uint16 MUX7:1;                      
    Uint16 MUX8:1;                      
    Uint16 MUX9:1;                      
    Uint16 MUX10:1;                     
    Uint16 MUX11:1;                     
    Uint16 MUX12:1;                     
    Uint16 MUX13:1;                     
    Uint16 MUX14:1;                     
    Uint16 MUX15:1;                     
    Uint16 MUX16:1;                     
    Uint16 MUX17:1;                     
    Uint16 MUX18:1;                     
    Uint16 MUX19:1;                     
    Uint16 MUX20:1;                     
    Uint16 MUX21:1;                     
    Uint16 MUX22:1;                     
    Uint16 MUX23:1;                     
    Uint16 MUX24:1;                     
    Uint16 MUX25:1;                     
    Uint16 MUX26:1;                     
    Uint16 MUX27:1;                     
    Uint16 MUX28:1;                     
    Uint16 MUX29:1;                     
    Uint16 MUX30:1;                     
    Uint16 MUX31:1;                     
};

union OUTPUT3MUXENABLE_REG {
    Uint32  all;
    struct  OUTPUT3MUXENABLE_BITS  bit;
};

struct OUTPUT4MUXENABLE_BITS {          
    Uint16 MUX0:1;                      
    Uint16 MUX1:1;                      
    Uint16 MUX2:1;                      
    Uint16 MUX3:1;                      
    Uint16 MUX4:1;                      
    Uint16 MUX5:1;                      
    Uint16 MUX6:1;                      
    Uint16 MUX7:1;                      
    Uint16 MUX8:1;                      
    Uint16 MUX9:1;                      
    Uint16 MUX10:1;                     
    Uint16 MUX11:1;                     
    Uint16 MUX12:1;                     
    Uint16 MUX13:1;                     
    Uint16 MUX14:1;                     
    Uint16 MUX15:1;                     
    Uint16 MUX16:1;                     
    Uint16 MUX17:1;                     
    Uint16 MUX18:1;                     
    Uint16 MUX19:1;                     
    Uint16 MUX20:1;                     
    Uint16 MUX21:1;                     
    Uint16 MUX22:1;                     
    Uint16 MUX23:1;                     
    Uint16 MUX24:1;                     
    Uint16 MUX25:1;                     
    Uint16 MUX26:1;                     
    Uint16 MUX27:1;                     
    Uint16 MUX28:1;                     
    Uint16 MUX29:1;                     
    Uint16 MUX30:1;                     
    Uint16 MUX31:1;                     
};

union OUTPUT4MUXENABLE_REG {
    Uint32  all;
    struct  OUTPUT4MUXENABLE_BITS  bit;
};

struct OUTPUT5MUXENABLE_BITS {          
    Uint16 MUX0:1;                      
    Uint16 MUX1:1;                      
    Uint16 MUX2:1;                      
    Uint16 MUX3:1;                      
    Uint16 MUX4:1;                      
    Uint16 MUX5:1;                      
    Uint16 MUX6:1;                      
    Uint16 MUX7:1;                      
    Uint16 MUX8:1;                      
    Uint16 MUX9:1;                      
    Uint16 MUX10:1;                     
    Uint16 MUX11:1;                     
    Uint16 MUX12:1;                     
    Uint16 MUX13:1;                     
    Uint16 MUX14:1;                     
    Uint16 MUX15:1;                     
    Uint16 MUX16:1;                     
    Uint16 MUX17:1;                     
    Uint16 MUX18:1;                     
    Uint16 MUX19:1;                     
    Uint16 MUX20:1;                     
    Uint16 MUX21:1;                     
    Uint16 MUX22:1;                     
    Uint16 MUX23:1;                     
    Uint16 MUX24:1;                     
    Uint16 MUX25:1;                     
    Uint16 MUX26:1;                     
    Uint16 MUX27:1;                     
    Uint16 MUX28:1;                     
    Uint16 MUX29:1;                     
    Uint16 MUX30:1;                     
    Uint16 MUX31:1;                     
};

union OUTPUT5MUXENABLE_REG {
    Uint32  all;
    struct  OUTPUT5MUXENABLE_BITS  bit;
};

struct OUTPUT6MUXENABLE_BITS {          
    Uint16 MUX0:1;                      
    Uint16 MUX1:1;                      
    Uint16 MUX2:1;                      
    Uint16 MUX3:1;                      
    Uint16 MUX4:1;                      
    Uint16 MUX5:1;                      
    Uint16 MUX6:1;                      
    Uint16 MUX7:1;                      
    Uint16 MUX8:1;                      
    Uint16 MUX9:1;                      
    Uint16 MUX10:1;                     
    Uint16 MUX11:1;                     
    Uint16 MUX12:1;                     
    Uint16 MUX13:1;                     
    Uint16 MUX14:1;                     
    Uint16 MUX15:1;                     
    Uint16 MUX16:1;                     
    Uint16 MUX17:1;                     
    Uint16 MUX18:1;                     
    Uint16 MUX19:1;                     
    Uint16 MUX20:1;                     
    Uint16 MUX21:1;                     
    Uint16 MUX22:1;                     
    Uint16 MUX23:1;                     
    Uint16 MUX24:1;                     
    Uint16 MUX25:1;                     
    Uint16 MUX26:1;                     
    Uint16 MUX27:1;                     
    Uint16 MUX28:1;                     
    Uint16 MUX29:1;                     
    Uint16 MUX30:1;                     
    Uint16 MUX31:1;                     
};

union OUTPUT6MUXENABLE_REG {
    Uint32  all;
    struct  OUTPUT6MUXENABLE_BITS  bit;
};

struct OUTPUT7MUXENABLE_BITS {          
    Uint16 MUX0:1;                      
    Uint16 MUX1:1;                      
    Uint16 MUX2:1;                      
    Uint16 MUX3:1;                      
    Uint16 MUX4:1;                      
    Uint16 MUX5:1;                      
    Uint16 MUX6:1;                      
    Uint16 MUX7:1;                      
    Uint16 MUX8:1;                      
    Uint16 MUX9:1;                      
    Uint16 MUX10:1;                     
    Uint16 MUX11:1;                     
    Uint16 MUX12:1;                     
    Uint16 MUX13:1;                     
    Uint16 MUX14:1;                     
    Uint16 MUX15:1;                     
    Uint16 MUX16:1;                     
    Uint16 MUX17:1;                     
    Uint16 MUX18:1;                     
    Uint16 MUX19:1;                     
    Uint16 MUX20:1;                     
    Uint16 MUX21:1;                     
    Uint16 MUX22:1;                     
    Uint16 MUX23:1;                     
    Uint16 MUX24:1;                     
    Uint16 MUX25:1;                     
    Uint16 MUX26:1;                     
    Uint16 MUX27:1;                     
    Uint16 MUX28:1;                     
    Uint16 MUX29:1;                     
    Uint16 MUX30:1;                     
    Uint16 MUX31:1;                     
};

union OUTPUT7MUXENABLE_REG {
    Uint32  all;
    struct  OUTPUT7MUXENABLE_BITS  bit;
};

struct OUTPUT8MUXENABLE_BITS {          
    Uint16 MUX0:1;                      
    Uint16 MUX1:1;                      
    Uint16 MUX2:1;                      
    Uint16 MUX3:1;                      
    Uint16 MUX4:1;                      
    Uint16 MUX5:1;                      
    Uint16 MUX6:1;                      
    Uint16 MUX7:1;                      
    Uint16 MUX8:1;                      
    Uint16 MUX9:1;                      
    Uint16 MUX10:1;                     
    Uint16 MUX11:1;                     
    Uint16 MUX12:1;                     
    Uint16 MUX13:1;                     
    Uint16 MUX14:1;                     
    Uint16 MUX15:1;                     
    Uint16 MUX16:1;                     
    Uint16 MUX17:1;                     
    Uint16 MUX18:1;                     
    Uint16 MUX19:1;                     
    Uint16 MUX20:1;                     
    Uint16 MUX21:1;                     
    Uint16 MUX22:1;                     
    Uint16 MUX23:1;                     
    Uint16 MUX24:1;                     
    Uint16 MUX25:1;                     
    Uint16 MUX26:1;                     
    Uint16 MUX27:1;                     
    Uint16 MUX28:1;                     
    Uint16 MUX29:1;                     
    Uint16 MUX30:1;                     
    Uint16 MUX31:1;                     
};

union OUTPUT8MUXENABLE_REG {
    Uint32  all;
    struct  OUTPUT8MUXENABLE_BITS  bit;
};

struct OUTPUTLATCH_BITS {               
    Uint16 OUTPUT1:1;                   
    Uint16 OUTPUT2:1;                   
    Uint16 OUTPUT3:1;                   
    Uint16 OUTPUT4:1;                   
    Uint16 OUTPUT5:1;                   
    Uint16 OUTPUT6:1;                   
    Uint16 OUTPUT7:1;                   
    Uint16 OUTPUT8:1;                   
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union OUTPUTLATCH_REG {
    Uint32  all;
    struct  OUTPUTLATCH_BITS  bit;
};

struct OUTPUTLATCHCLR_BITS {            
    Uint16 OUTPUT1:1;                   
    Uint16 OUTPUT2:1;                   
    Uint16 OUTPUT3:1;                   
    Uint16 OUTPUT4:1;                   
    Uint16 OUTPUT5:1;                   
    Uint16 OUTPUT6:1;                   
    Uint16 OUTPUT7:1;                   
    Uint16 OUTPUT8:1;                   
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union OUTPUTLATCHCLR_REG {
    Uint32  all;
    struct  OUTPUTLATCHCLR_BITS  bit;
};

struct OUTPUTLATCHFRC_BITS {            
    Uint16 OUTPUT1:1;                   
    Uint16 OUTPUT2:1;                   
    Uint16 OUTPUT3:1;                   
    Uint16 OUTPUT4:1;                   
    Uint16 OUTPUT5:1;                   
    Uint16 OUTPUT6:1;                   
    Uint16 OUTPUT7:1;                   
    Uint16 OUTPUT8:1;                   
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union OUTPUTLATCHFRC_REG {
    Uint32  all;
    struct  OUTPUTLATCHFRC_BITS  bit;
};

struct OUTPUTLATCHENABLE_BITS {         
    Uint16 OUTPUT1:1;                   
    Uint16 OUTPUT2:1;                   
    Uint16 OUTPUT3:1;                   
    Uint16 OUTPUT4:1;                   
    Uint16 OUTPUT5:1;                   
    Uint16 OUTPUT6:1;                   
    Uint16 OUTPUT7:1;                   
    Uint16 OUTPUT8:1;                   
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union OUTPUTLATCHENABLE_REG {
    Uint32  all;
    struct  OUTPUTLATCHENABLE_BITS  bit;
};

struct OUTPUTINV_BITS {                 
    Uint16 OUTPUT1:1;                   
    Uint16 OUTPUT2:1;                   
    Uint16 OUTPUT3:1;                   
    Uint16 OUTPUT4:1;                   
    Uint16 OUTPUT5:1;                   
    Uint16 OUTPUT6:1;                   
    Uint16 OUTPUT7:1;                   
    Uint16 OUTPUT8:1;                   
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union OUTPUTINV_REG {
    Uint32  all;
    struct  OUTPUTINV_BITS  bit;
};

struct OUTPUTLOCK_BITS {                
    Uint16 LOCK:1;                      
    Uint16 rsvd1:15;                    
    Uint16 KEY:16;                      
};

union OUTPUTLOCK_REG {
    Uint32  all;
    struct  OUTPUTLOCK_BITS  bit;
};

struct OUTPUT_XBAR_REGS {
    union   OUTPUT1MUX0TO15CFG_REG           OUTPUT1MUX0TO15CFG;           
    union   OUTPUT1MUX16TO31CFG_REG          OUTPUT1MUX16TO31CFG;          
    union   OUTPUT2MUX0TO15CFG_REG           OUTPUT2MUX0TO15CFG;           
    union   OUTPUT2MUX16TO31CFG_REG          OUTPUT2MUX16TO31CFG;          
    union   OUTPUT3MUX0TO15CFG_REG           OUTPUT3MUX0TO15CFG;           
    union   OUTPUT3MUX16TO31CFG_REG          OUTPUT3MUX16TO31CFG;          
    union   OUTPUT4MUX0TO15CFG_REG           OUTPUT4MUX0TO15CFG;           
    union   OUTPUT4MUX16TO31CFG_REG          OUTPUT4MUX16TO31CFG;          
    union   OUTPUT5MUX0TO15CFG_REG           OUTPUT5MUX0TO15CFG;           
    union   OUTPUT5MUX16TO31CFG_REG          OUTPUT5MUX16TO31CFG;          
    union   OUTPUT6MUX0TO15CFG_REG           OUTPUT6MUX0TO15CFG;           
    union   OUTPUT6MUX16TO31CFG_REG          OUTPUT6MUX16TO31CFG;          
    union   OUTPUT7MUX0TO15CFG_REG           OUTPUT7MUX0TO15CFG;           
    union   OUTPUT7MUX16TO31CFG_REG          OUTPUT7MUX16TO31CFG;          
    union   OUTPUT8MUX0TO15CFG_REG           OUTPUT8MUX0TO15CFG;           
    union   OUTPUT8MUX16TO31CFG_REG          OUTPUT8MUX16TO31CFG;          
    union   OUTPUT1MUXENABLE_REG             OUTPUT1MUXENABLE;             
    union   OUTPUT2MUXENABLE_REG             OUTPUT2MUXENABLE;             
    union   OUTPUT3MUXENABLE_REG             OUTPUT3MUXENABLE;             
    union   OUTPUT4MUXENABLE_REG             OUTPUT4MUXENABLE;             
    union   OUTPUT5MUXENABLE_REG             OUTPUT5MUXENABLE;             
    union   OUTPUT6MUXENABLE_REG             OUTPUT6MUXENABLE;             
    union   OUTPUT7MUXENABLE_REG             OUTPUT7MUXENABLE;             
    union   OUTPUT8MUXENABLE_REG             OUTPUT8MUXENABLE;             
    union   OUTPUTLATCH_REG                  OUTPUTLATCH;                  
    union   OUTPUTLATCHCLR_REG               OUTPUTLATCHCLR;               
    union   OUTPUTLATCHFRC_REG               OUTPUTLATCHFRC;               
    union   OUTPUTLATCHENABLE_REG            OUTPUTLATCHENABLE;            
    union   OUTPUTINV_REG                    OUTPUTINV;                    
    Uint16                                   rsvd1[4];                     
    union   OUTPUTLOCK_REG                   OUTPUTLOCK;                   
};




extern volatile struct OUTPUT_XBAR_REGS OutputXbarRegs;
extern volatile struct OUTPUT_XBAR_REGS ClbOutputXbarRegs;



















































struct PIECTRL_BITS {                   
    Uint16 ENPIE:1;                     
    Uint16 PIEVECT:15;                  
};

union PIECTRL_REG {
    Uint16  all;
    struct  PIECTRL_BITS  bit;
};

struct PIEACK_BITS {                    
    Uint16 ACK1:1;                      
    Uint16 ACK2:1;                      
    Uint16 ACK3:1;                      
    Uint16 ACK4:1;                      
    Uint16 ACK5:1;                      
    Uint16 ACK6:1;                      
    Uint16 ACK7:1;                      
    Uint16 ACK8:1;                      
    Uint16 ACK9:1;                      
    Uint16 ACK10:1;                     
    Uint16 ACK11:1;                     
    Uint16 ACK12:1;                     
    Uint16 rsvd1:4;                     
};

union PIEACK_REG {
    Uint16  all;
    struct  PIEACK_BITS  bit;
};

struct PIEIER1_BITS {                   
    Uint16 INTx1:1;                     
    Uint16 INTx2:1;                     
    Uint16 INTx3:1;                     
    Uint16 INTx4:1;                     
    Uint16 INTx5:1;                     
    Uint16 INTx6:1;                     
    Uint16 INTx7:1;                     
    Uint16 INTx8:1;                     
    Uint16 INTx9:1;                     
    Uint16 INTx10:1;                    
    Uint16 INTx11:1;                    
    Uint16 INTx12:1;                    
    Uint16 INTx13:1;                    
    Uint16 INTx14:1;                    
    Uint16 INTx15:1;                    
    Uint16 INTx16:1;                    
};

union PIEIER1_REG {
    Uint16  all;
    struct  PIEIER1_BITS  bit;
};

struct PIEIFR1_BITS {                   
    Uint16 INTx1:1;                     
    Uint16 INTx2:1;                     
    Uint16 INTx3:1;                     
    Uint16 INTx4:1;                     
    Uint16 INTx5:1;                     
    Uint16 INTx6:1;                     
    Uint16 INTx7:1;                     
    Uint16 INTx8:1;                     
    Uint16 INTx9:1;                     
    Uint16 INTx10:1;                    
    Uint16 INTx11:1;                    
    Uint16 INTx12:1;                    
    Uint16 INTx13:1;                    
    Uint16 INTx14:1;                    
    Uint16 INTx15:1;                    
    Uint16 INTx16:1;                    
};

union PIEIFR1_REG {
    Uint16  all;
    struct  PIEIFR1_BITS  bit;
};

struct PIEIER2_BITS {                   
    Uint16 INTx1:1;                     
    Uint16 INTx2:1;                     
    Uint16 INTx3:1;                     
    Uint16 INTx4:1;                     
    Uint16 INTx5:1;                     
    Uint16 INTx6:1;                     
    Uint16 INTx7:1;                     
    Uint16 INTx8:1;                     
    Uint16 INTx9:1;                     
    Uint16 INTx10:1;                    
    Uint16 INTx11:1;                    
    Uint16 INTx12:1;                    
    Uint16 INTx13:1;                    
    Uint16 INTx14:1;                    
    Uint16 INTx15:1;                    
    Uint16 INTx16:1;                    
};

union PIEIER2_REG {
    Uint16  all;
    struct  PIEIER2_BITS  bit;
};

struct PIEIFR2_BITS {                   
    Uint16 INTx1:1;                     
    Uint16 INTx2:1;                     
    Uint16 INTx3:1;                     
    Uint16 INTx4:1;                     
    Uint16 INTx5:1;                     
    Uint16 INTx6:1;                     
    Uint16 INTx7:1;                     
    Uint16 INTx8:1;                     
    Uint16 INTx9:1;                     
    Uint16 INTx10:1;                    
    Uint16 INTx11:1;                    
    Uint16 INTx12:1;                    
    Uint16 INTx13:1;                    
    Uint16 INTx14:1;                    
    Uint16 INTx15:1;                    
    Uint16 INTx16:1;                    
};

union PIEIFR2_REG {
    Uint16  all;
    struct  PIEIFR2_BITS  bit;
};

struct PIEIER3_BITS {                   
    Uint16 INTx1:1;                     
    Uint16 INTx2:1;                     
    Uint16 INTx3:1;                     
    Uint16 INTx4:1;                     
    Uint16 INTx5:1;                     
    Uint16 INTx6:1;                     
    Uint16 INTx7:1;                     
    Uint16 INTx8:1;                     
    Uint16 INTx9:1;                     
    Uint16 INTx10:1;                    
    Uint16 INTx11:1;                    
    Uint16 INTx12:1;                    
    Uint16 INTx13:1;                    
    Uint16 INTx14:1;                    
    Uint16 INTx15:1;                    
    Uint16 INTx16:1;                    
};

union PIEIER3_REG {
    Uint16  all;
    struct  PIEIER3_BITS  bit;
};

struct PIEIFR3_BITS {                   
    Uint16 INTx1:1;                     
    Uint16 INTx2:1;                     
    Uint16 INTx3:1;                     
    Uint16 INTx4:1;                     
    Uint16 INTx5:1;                     
    Uint16 INTx6:1;                     
    Uint16 INTx7:1;                     
    Uint16 INTx8:1;                     
    Uint16 INTx9:1;                     
    Uint16 INTx10:1;                    
    Uint16 INTx11:1;                    
    Uint16 INTx12:1;                    
    Uint16 INTx13:1;                    
    Uint16 INTx14:1;                    
    Uint16 INTx15:1;                    
    Uint16 INTx16:1;                    
};

union PIEIFR3_REG {
    Uint16  all;
    struct  PIEIFR3_BITS  bit;
};

struct PIEIER4_BITS {                   
    Uint16 INTx1:1;                     
    Uint16 INTx2:1;                     
    Uint16 INTx3:1;                     
    Uint16 INTx4:1;                     
    Uint16 INTx5:1;                     
    Uint16 INTx6:1;                     
    Uint16 INTx7:1;                     
    Uint16 INTx8:1;                     
    Uint16 INTx9:1;                     
    Uint16 INTx10:1;                    
    Uint16 INTx11:1;                    
    Uint16 INTx12:1;                    
    Uint16 INTx13:1;                    
    Uint16 INTx14:1;                    
    Uint16 INTx15:1;                    
    Uint16 INTx16:1;                    
};

union PIEIER4_REG {
    Uint16  all;
    struct  PIEIER4_BITS  bit;
};

struct PIEIFR4_BITS {                   
    Uint16 INTx1:1;                     
    Uint16 INTx2:1;                     
    Uint16 INTx3:1;                     
    Uint16 INTx4:1;                     
    Uint16 INTx5:1;                     
    Uint16 INTx6:1;                     
    Uint16 INTx7:1;                     
    Uint16 INTx8:1;                     
    Uint16 INTx9:1;                     
    Uint16 INTx10:1;                    
    Uint16 INTx11:1;                    
    Uint16 INTx12:1;                    
    Uint16 INTx13:1;                    
    Uint16 INTx14:1;                    
    Uint16 INTx15:1;                    
    Uint16 INTx16:1;                    
};

union PIEIFR4_REG {
    Uint16  all;
    struct  PIEIFR4_BITS  bit;
};

struct PIEIER5_BITS {                   
    Uint16 INTx1:1;                     
    Uint16 INTx2:1;                     
    Uint16 INTx3:1;                     
    Uint16 INTx4:1;                     
    Uint16 INTx5:1;                     
    Uint16 INTx6:1;                     
    Uint16 INTx7:1;                     
    Uint16 INTx8:1;                     
    Uint16 INTx9:1;                     
    Uint16 INTx10:1;                    
    Uint16 INTx11:1;                    
    Uint16 INTx12:1;                    
    Uint16 INTx13:1;                    
    Uint16 INTx14:1;                    
    Uint16 INTx15:1;                    
    Uint16 INTx16:1;                    
};

union PIEIER5_REG {
    Uint16  all;
    struct  PIEIER5_BITS  bit;
};

struct PIEIFR5_BITS {                   
    Uint16 INTx1:1;                     
    Uint16 INTx2:1;                     
    Uint16 INTx3:1;                     
    Uint16 INTx4:1;                     
    Uint16 INTx5:1;                     
    Uint16 INTx6:1;                     
    Uint16 INTx7:1;                     
    Uint16 INTx8:1;                     
    Uint16 INTx9:1;                     
    Uint16 INTx10:1;                    
    Uint16 INTx11:1;                    
    Uint16 INTx12:1;                    
    Uint16 INTx13:1;                    
    Uint16 INTx14:1;                    
    Uint16 INTx15:1;                    
    Uint16 INTx16:1;                    
};

union PIEIFR5_REG {
    Uint16  all;
    struct  PIEIFR5_BITS  bit;
};

struct PIEIER6_BITS {                   
    Uint16 INTx1:1;                     
    Uint16 INTx2:1;                     
    Uint16 INTx3:1;                     
    Uint16 INTx4:1;                     
    Uint16 INTx5:1;                     
    Uint16 INTx6:1;                     
    Uint16 INTx7:1;                     
    Uint16 INTx8:1;                     
    Uint16 INTx9:1;                     
    Uint16 INTx10:1;                    
    Uint16 INTx11:1;                    
    Uint16 INTx12:1;                    
    Uint16 INTx13:1;                    
    Uint16 INTx14:1;                    
    Uint16 INTx15:1;                    
    Uint16 INTx16:1;                    
};

union PIEIER6_REG {
    Uint16  all;
    struct  PIEIER6_BITS  bit;
};

struct PIEIFR6_BITS {                   
    Uint16 INTx1:1;                     
    Uint16 INTx2:1;                     
    Uint16 INTx3:1;                     
    Uint16 INTx4:1;                     
    Uint16 INTx5:1;                     
    Uint16 INTx6:1;                     
    Uint16 INTx7:1;                     
    Uint16 INTx8:1;                     
    Uint16 INTx9:1;                     
    Uint16 INTx10:1;                    
    Uint16 INTx11:1;                    
    Uint16 INTx12:1;                    
    Uint16 INTx13:1;                    
    Uint16 INTx14:1;                    
    Uint16 INTx15:1;                    
    Uint16 INTx16:1;                    
};

union PIEIFR6_REG {
    Uint16  all;
    struct  PIEIFR6_BITS  bit;
};

struct PIEIER7_BITS {                   
    Uint16 INTx1:1;                     
    Uint16 INTx2:1;                     
    Uint16 INTx3:1;                     
    Uint16 INTx4:1;                     
    Uint16 INTx5:1;                     
    Uint16 INTx6:1;                     
    Uint16 INTx7:1;                     
    Uint16 INTx8:1;                     
    Uint16 INTx9:1;                     
    Uint16 INTx10:1;                    
    Uint16 INTx11:1;                    
    Uint16 INTx12:1;                    
    Uint16 INTx13:1;                    
    Uint16 INTx14:1;                    
    Uint16 INTx15:1;                    
    Uint16 INTx16:1;                    
};

union PIEIER7_REG {
    Uint16  all;
    struct  PIEIER7_BITS  bit;
};

struct PIEIFR7_BITS {                   
    Uint16 INTx1:1;                     
    Uint16 INTx2:1;                     
    Uint16 INTx3:1;                     
    Uint16 INTx4:1;                     
    Uint16 INTx5:1;                     
    Uint16 INTx6:1;                     
    Uint16 INTx7:1;                     
    Uint16 INTx8:1;                     
    Uint16 INTx9:1;                     
    Uint16 INTx10:1;                    
    Uint16 INTx11:1;                    
    Uint16 INTx12:1;                    
    Uint16 INTx13:1;                    
    Uint16 INTx14:1;                    
    Uint16 INTx15:1;                    
    Uint16 INTx16:1;                    
};

union PIEIFR7_REG {
    Uint16  all;
    struct  PIEIFR7_BITS  bit;
};

struct PIEIER8_BITS {                   
    Uint16 INTx1:1;                     
    Uint16 INTx2:1;                     
    Uint16 INTx3:1;                     
    Uint16 INTx4:1;                     
    Uint16 INTx5:1;                     
    Uint16 INTx6:1;                     
    Uint16 INTx7:1;                     
    Uint16 INTx8:1;                     
    Uint16 INTx9:1;                     
    Uint16 INTx10:1;                    
    Uint16 INTx11:1;                    
    Uint16 INTx12:1;                    
    Uint16 INTx13:1;                    
    Uint16 INTx14:1;                    
    Uint16 INTx15:1;                    
    Uint16 INTx16:1;                    
};

union PIEIER8_REG {
    Uint16  all;
    struct  PIEIER8_BITS  bit;
};

struct PIEIFR8_BITS {                   
    Uint16 INTx1:1;                     
    Uint16 INTx2:1;                     
    Uint16 INTx3:1;                     
    Uint16 INTx4:1;                     
    Uint16 INTx5:1;                     
    Uint16 INTx6:1;                     
    Uint16 INTx7:1;                     
    Uint16 INTx8:1;                     
    Uint16 INTx9:1;                     
    Uint16 INTx10:1;                    
    Uint16 INTx11:1;                    
    Uint16 INTx12:1;                    
    Uint16 INTx13:1;                    
    Uint16 INTx14:1;                    
    Uint16 INTx15:1;                    
    Uint16 INTx16:1;                    
};

union PIEIFR8_REG {
    Uint16  all;
    struct  PIEIFR8_BITS  bit;
};

struct PIEIER9_BITS {                   
    Uint16 INTx1:1;                     
    Uint16 INTx2:1;                     
    Uint16 INTx3:1;                     
    Uint16 INTx4:1;                     
    Uint16 INTx5:1;                     
    Uint16 INTx6:1;                     
    Uint16 INTx7:1;                     
    Uint16 INTx8:1;                     
    Uint16 INTx9:1;                     
    Uint16 INTx10:1;                    
    Uint16 INTx11:1;                    
    Uint16 INTx12:1;                    
    Uint16 INTx13:1;                    
    Uint16 INTx14:1;                    
    Uint16 INTx15:1;                    
    Uint16 INTx16:1;                    
};

union PIEIER9_REG {
    Uint16  all;
    struct  PIEIER9_BITS  bit;
};

struct PIEIFR9_BITS {                   
    Uint16 INTx1:1;                     
    Uint16 INTx2:1;                     
    Uint16 INTx3:1;                     
    Uint16 INTx4:1;                     
    Uint16 INTx5:1;                     
    Uint16 INTx6:1;                     
    Uint16 INTx7:1;                     
    Uint16 INTx8:1;                     
    Uint16 INTx9:1;                     
    Uint16 INTx10:1;                    
    Uint16 INTx11:1;                    
    Uint16 INTx12:1;                    
    Uint16 INTx13:1;                    
    Uint16 INTx14:1;                    
    Uint16 INTx15:1;                    
    Uint16 INTx16:1;                    
};

union PIEIFR9_REG {
    Uint16  all;
    struct  PIEIFR9_BITS  bit;
};

struct PIEIER10_BITS {                  
    Uint16 INTx1:1;                     
    Uint16 INTx2:1;                     
    Uint16 INTx3:1;                     
    Uint16 INTx4:1;                     
    Uint16 INTx5:1;                     
    Uint16 INTx6:1;                     
    Uint16 INTx7:1;                     
    Uint16 INTx8:1;                     
    Uint16 INTx9:1;                     
    Uint16 INTx10:1;                    
    Uint16 INTx11:1;                    
    Uint16 INTx12:1;                    
    Uint16 INTx13:1;                    
    Uint16 INTx14:1;                    
    Uint16 INTx15:1;                    
    Uint16 INTx16:1;                    
};

union PIEIER10_REG {
    Uint16  all;
    struct  PIEIER10_BITS  bit;
};

struct PIEIFR10_BITS {                  
    Uint16 INTx1:1;                     
    Uint16 INTx2:1;                     
    Uint16 INTx3:1;                     
    Uint16 INTx4:1;                     
    Uint16 INTx5:1;                     
    Uint16 INTx6:1;                     
    Uint16 INTx7:1;                     
    Uint16 INTx8:1;                     
    Uint16 INTx9:1;                     
    Uint16 INTx10:1;                    
    Uint16 INTx11:1;                    
    Uint16 INTx12:1;                    
    Uint16 INTx13:1;                    
    Uint16 INTx14:1;                    
    Uint16 INTx15:1;                    
    Uint16 INTx16:1;                    
};

union PIEIFR10_REG {
    Uint16  all;
    struct  PIEIFR10_BITS  bit;
};

struct PIEIER11_BITS {                  
    Uint16 INTx1:1;                     
    Uint16 INTx2:1;                     
    Uint16 INTx3:1;                     
    Uint16 INTx4:1;                     
    Uint16 INTx5:1;                     
    Uint16 INTx6:1;                     
    Uint16 INTx7:1;                     
    Uint16 INTx8:1;                     
    Uint16 INTx9:1;                     
    Uint16 INTx10:1;                    
    Uint16 INTx11:1;                    
    Uint16 INTx12:1;                    
    Uint16 INTx13:1;                    
    Uint16 INTx14:1;                    
    Uint16 INTx15:1;                    
    Uint16 INTx16:1;                    
};

union PIEIER11_REG {
    Uint16  all;
    struct  PIEIER11_BITS  bit;
};

struct PIEIFR11_BITS {                  
    Uint16 INTx1:1;                     
    Uint16 INTx2:1;                     
    Uint16 INTx3:1;                     
    Uint16 INTx4:1;                     
    Uint16 INTx5:1;                     
    Uint16 INTx6:1;                     
    Uint16 INTx7:1;                     
    Uint16 INTx8:1;                     
    Uint16 INTx9:1;                     
    Uint16 INTx10:1;                    
    Uint16 INTx11:1;                    
    Uint16 INTx12:1;                    
    Uint16 INTx13:1;                    
    Uint16 INTx14:1;                    
    Uint16 INTx15:1;                    
    Uint16 INTx16:1;                    
};

union PIEIFR11_REG {
    Uint16  all;
    struct  PIEIFR11_BITS  bit;
};

struct PIEIER12_BITS {                  
    Uint16 INTx1:1;                     
    Uint16 INTx2:1;                     
    Uint16 INTx3:1;                     
    Uint16 INTx4:1;                     
    Uint16 INTx5:1;                     
    Uint16 INTx6:1;                     
    Uint16 INTx7:1;                     
    Uint16 INTx8:1;                     
    Uint16 INTx9:1;                     
    Uint16 INTx10:1;                    
    Uint16 INTx11:1;                    
    Uint16 INTx12:1;                    
    Uint16 INTx13:1;                    
    Uint16 INTx14:1;                    
    Uint16 INTx15:1;                    
    Uint16 INTx16:1;                    
};

union PIEIER12_REG {
    Uint16  all;
    struct  PIEIER12_BITS  bit;
};

struct PIEIFR12_BITS {                  
    Uint16 INTx1:1;                     
    Uint16 INTx2:1;                     
    Uint16 INTx3:1;                     
    Uint16 INTx4:1;                     
    Uint16 INTx5:1;                     
    Uint16 INTx6:1;                     
    Uint16 INTx7:1;                     
    Uint16 INTx8:1;                     
    Uint16 INTx9:1;                     
    Uint16 INTx10:1;                    
    Uint16 INTx11:1;                    
    Uint16 INTx12:1;                    
    Uint16 INTx13:1;                    
    Uint16 INTx14:1;                    
    Uint16 INTx15:1;                    
    Uint16 INTx16:1;                    
};

union PIEIFR12_REG {
    Uint16  all;
    struct  PIEIFR12_BITS  bit;
};

struct PIE_CTRL_REGS {
    union   PIECTRL_REG                      PIECTRL;                      
    union   PIEACK_REG                       PIEACK;                       
    union   PIEIER1_REG                      PIEIER1;                      
    union   PIEIFR1_REG                      PIEIFR1;                      
    union   PIEIER2_REG                      PIEIER2;                      
    union   PIEIFR2_REG                      PIEIFR2;                      
    union   PIEIER3_REG                      PIEIER3;                      
    union   PIEIFR3_REG                      PIEIFR3;                      
    union   PIEIER4_REG                      PIEIER4;                      
    union   PIEIFR4_REG                      PIEIFR4;                      
    union   PIEIER5_REG                      PIEIER5;                      
    union   PIEIFR5_REG                      PIEIFR5;                      
    union   PIEIER6_REG                      PIEIER6;                      
    union   PIEIFR6_REG                      PIEIFR6;                      
    union   PIEIER7_REG                      PIEIER7;                      
    union   PIEIFR7_REG                      PIEIFR7;                      
    union   PIEIER8_REG                      PIEIER8;                      
    union   PIEIFR8_REG                      PIEIFR8;                      
    union   PIEIER9_REG                      PIEIER9;                      
    union   PIEIFR9_REG                      PIEIFR9;                      
    union   PIEIER10_REG                     PIEIER10;                     
    union   PIEIFR10_REG                     PIEIFR10;                     
    union   PIEIER11_REG                     PIEIER11;                     
    union   PIEIFR11_REG                     PIEIFR11;                     
    union   PIEIER12_REG                     PIEIER12;                     
    union   PIEIFR12_REG                     PIEIFR12;                     
};




extern volatile struct PIE_CTRL_REGS PieCtrlRegs;


















































typedef __interrupt void (*PINT)(void);


struct PIE_VECT_TABLE {
    PINT PIE1_RESERVED_INT;         
    PINT PIE2_RESERVED_INT;         
    PINT PIE3_RESERVED_INT;         
    PINT PIE4_RESERVED_INT;         
    PINT PIE5_RESERVED_INT;         
    PINT PIE6_RESERVED_INT;         
    PINT PIE7_RESERVED_INT;         
    PINT PIE8_RESERVED_INT;         
    PINT PIE9_RESERVED_INT;         
    PINT PIE10_RESERVED_INT;        
    PINT PIE11_RESERVED_INT;        
    PINT PIE12_RESERVED_INT;        
    PINT PIE13_RESERVED_INT;        
    PINT TIMER1_INT;                
    PINT TIMER2_INT;                
    PINT DATALOG_INT;               
    PINT RTOS_INT;                  
    PINT EMU_INT;                   
    PINT NMI_INT;                   
    PINT ILLEGAL_INT;               
    PINT USER1_INT;                 
    PINT USER2_INT;                 
    PINT USER3_INT;                 
    PINT USER4_INT;                 
    PINT USER5_INT;                 
    PINT USER6_INT;                 
    PINT USER7_INT;                 
    PINT USER8_INT;                 
    PINT USER9_INT;                 
    PINT USER10_INT;                
    PINT USER11_INT;                
    PINT USER12_INT;                
    PINT ADCA1_INT;                 
    PINT ADCB1_INT;                 
    PINT ADCC1_INT;                 
    PINT XINT1_INT;                 
    PINT XINT2_INT;                 
    PINT PIE14_RESERVED_INT;        
    PINT TIMER0_INT;                
    PINT WAKE_INT;                  
    PINT EPWM1_TZ_INT;              
    PINT EPWM2_TZ_INT;              
    PINT EPWM3_TZ_INT;              
    PINT EPWM4_TZ_INT;              
    PINT EPWM5_TZ_INT;              
    PINT EPWM6_TZ_INT;              
    PINT EPWM7_TZ_INT;              
    PINT EPWM8_TZ_INT;              
    PINT EPWM1_INT;                 
    PINT EPWM2_INT;                 
    PINT EPWM3_INT;                 
    PINT EPWM4_INT;                 
    PINT EPWM5_INT;                 
    PINT EPWM6_INT;                 
    PINT EPWM7_INT;                 
    PINT EPWM8_INT;                 
    PINT ECAP1_INT;                 
    PINT ECAP2_INT;                 
    PINT ECAP3_INT;                 
    PINT PIE15_RESERVED_INT;        
    PINT PIE16_RESERVED_INT;        
    PINT PIE17_RESERVED_INT;        
    PINT PIE18_RESERVED_INT;        
    PINT PIE19_RESERVED_INT;        
    PINT EQEP1_INT;                 
    PINT EQEP2_INT;                 
    PINT PIE20_RESERVED_INT;        
    PINT PIE21_RESERVED_INT;        
    PINT CLB1_INT;                  
    PINT CLB2_INT;                  
    PINT CLB3_INT;                  
    PINT CLB4_INT;                  
    PINT SPIA_RX_INT;               
    PINT SPIA_TX_INT;               
    PINT SPIB_RX_INT;               
    PINT SPIB_TX_INT;               
    PINT PIE22_RESERVED_INT;        
    PINT PIE23_RESERVED_INT;        
    PINT PIE24_RESERVED_INT;        
    PINT PIE25_RESERVED_INT;        
    PINT DMA_CH1_INT;               
    PINT DMA_CH2_INT;               
    PINT DMA_CH3_INT;               
    PINT DMA_CH4_INT;               
    PINT DMA_CH5_INT;               
    PINT DMA_CH6_INT;               
    PINT PIE26_RESERVED_INT;        
    PINT PIE27_RESERVED_INT;        
    PINT I2CA_INT;                  
    PINT I2CA_FIFO_INT;             
    PINT I2CB_INT;                  
    PINT I2CB_FIFO_INT;             
    PINT PIE28_RESERVED_INT;        
    PINT PIE29_RESERVED_INT;        
    PINT PIE30_RESERVED_INT;        
    PINT PIE31_RESERVED_INT;        
    PINT SCIA_RX_INT;               
    PINT SCIA_TX_INT;               
    PINT SCIB_RX_INT;               
    PINT SCIB_TX_INT;               
    PINT CANA0_INT;                 
    PINT CANA1_INT;                 
    PINT PIE32_RESERVED_INT;        
    PINT PIE33_RESERVED_INT;        
    PINT ADCA_EVT_INT;              
    PINT ADCA2_INT;                 
    PINT ADCA3_INT;                 
    PINT ADCA4_INT;                 
    PINT ADCB_EVT_INT;              
    PINT ADCB2_INT;                 
    PINT ADCB3_INT;                 
    PINT ADCB4_INT;                 
    PINT CLA1_1_INT;                
    PINT CLA1_2_INT;                
    PINT CLA1_3_INT;                
    PINT CLA1_4_INT;                
    PINT CLA1_5_INT;                
    PINT CLA1_6_INT;                
    PINT CLA1_7_INT;                
    PINT CLA1_8_INT;                
    PINT XINT3_INT;                 
    PINT XINT4_INT;                 
    PINT XINT5_INT;                 
    PINT MPOST_INT;                 
    PINT FMC_INT;                   
    PINT PIE34_RESERVED_INT;        
    PINT FPU_OVERFLOW_INT;          
    PINT FPU_UNDERFLOW_INT;         
    PINT PIE35_RESERVED_INT;        
    PINT SYS_ERR_INT;               
    PINT PIE36_RESERVED_INT;        
    PINT PIE37_RESERVED_INT;        
    PINT PIE38_RESERVED_INT;        
    PINT PIE39_RESERVED_INT;        
    PINT PIE40_RESERVED_INT;        
    PINT PIE41_RESERVED_INT;        
    PINT PIE42_RESERVED_INT;        
    PINT PIE43_RESERVED_INT;        
    PINT PIE44_RESERVED_INT;        
    PINT PIE45_RESERVED_INT;        
    PINT PIE46_RESERVED_INT;        
    PINT PIE47_RESERVED_INT;        
    PINT PIE48_RESERVED_INT;        
    PINT PIE49_RESERVED_INT;        
    PINT PIE50_RESERVED_INT;        
    PINT PIE51_RESERVED_INT;        
    PINT PIE52_RESERVED_INT;        
    PINT PIE53_RESERVED_INT;        
    PINT PIE54_RESERVED_INT;        
    PINT PIE55_RESERVED_INT;        
    PINT PIE56_RESERVED_INT;        
    PINT PIE57_RESERVED_INT;        
    PINT PIE58_RESERVED_INT;        
    PINT PIE59_RESERVED_INT;        
    PINT ECAP3_2_INT;               
    PINT PIE60_RESERVED_INT;        
    PINT PIE61_RESERVED_INT;        
    PINT PIE62_RESERVED_INT;        
    PINT PIE63_RESERVED_INT;        
    PINT PIE64_RESERVED_INT;        
    PINT SDFM1_INT;                 
    PINT SDFM2_INT;                 
    PINT PIE65_RESERVED_INT;        
    PINT PIE66_RESERVED_INT;        
    PINT SDFM1DR1_INT;              
    PINT SDFM1DR2_INT;              
    PINT SDFM1DR3_INT;              
    PINT SDFM1DR4_INT;              
    PINT PIE67_RESERVED_INT;        
    PINT PIE68_RESERVED_INT;        
    PINT PIE69_RESERVED_INT;        
    PINT PIE70_RESERVED_INT;        
    PINT SDFM2DR1_INT;              
    PINT SDFM2DR2_INT;              
    PINT SDFM2DR3_INT;              
    PINT SDFM2DR4_INT;              
    PINT PIE71_RESERVED_INT;        
    PINT PIE72_RESERVED_INT;        
    PINT FSITXA1_INT;               
    PINT FSITXA2_INT;               
    PINT FSIRXA1_INT;               
    PINT FSIRXA2_INT;               
    PINT PIE73_RESERVED_INT;        
    PINT DCC0_INT;                  
    PINT LINA_0_INT;                
    PINT LINA_1_INT;                
    PINT LINB_0_INT;                
    PINT LINB_1_INT;                
    PINT PMBUSA_INT;                
    PINT PIE74_RESERVED_INT;        
    PINT PIE75_RESERVED_INT;        
    PINT DCC1_INT;                  
    PINT MCANA_0_INT;               
    PINT MCANA_1_INT;               
    PINT MCANA_ECC_INT;             
    PINT MCANA_WAKE_INT;            
    PINT BGCRC_INT;                 
    PINT PIE76_RESERVED_INT;        
    PINT PIE77_RESERVED_INT;        
    PINT HICA_INT;                  
    PINT ADCC_EVT_INT;              
    PINT ADCC2_INT;                 
    PINT ADCC3_INT;                 
    PINT ADCC4_INT;                 
    PINT PIE78_RESERVED_INT;        
    PINT PIE79_RESERVED_INT;        
    PINT PIE80_RESERVED_INT;        
    PINT PIE81_RESERVED_INT;        
    PINT PIE82_RESERVED_INT;        
    PINT PIE83_RESERVED_INT;        
    PINT PIE84_RESERVED_INT;        
    PINT PIE85_RESERVED_INT;        
    PINT PIE86_RESERVED_INT;        
    PINT PIE87_RESERVED_INT;        
    PINT PIE88_RESERVED_INT;        
    PINT PIE89_RESERVED_INT;        
    PINT PIE90_RESERVED_INT;        
    PINT RAM_CORR_ERR_INT;          
    PINT FLASH_CORR_ERR_INT;        
    PINT RAM_ACC_VIOL_INT;          
    PINT AES_SINTREQUEST_INT;       
    PINT BGCRC_CLA1_INT;            
    PINT CLA_OVERFLOW_INT;          
    PINT CLA_UNDERFLOW_INT;         
};





extern volatile struct PIE_VECT_TABLE PieVectTable;




















































struct PMBMC_BITS {                     
    Uint16 RW:1;                        
    Uint16 SLAVE_ADDR:7;                
    Uint16 BYTE_COUNT:8;                
    Uint16 CMD_ENA:1;                   
    Uint16 EXT_CMD:1;                   
    Uint16 PEC_ENA:1;                   
    Uint16 GRP_CMD:1;                   
    Uint16 PRC_CALL:1;                  
    Uint16 rsvd1:11;                    
};

union PMBMC_REG {
    Uint32  all;
    struct  PMBMC_BITS  bit;
};

struct PMBACK_BITS {                    
    Uint16 ACK:1;                       
    Uint16 rsvd1:15;                    
    Uint16 rsvd2:16;                    
};

union PMBACK_REG {
    Uint32  all;
    struct  PMBACK_BITS  bit;
};

struct PMBSTS_BITS {                    
    Uint16 RD_BYTE_COUNT:3;             
    Uint16 DATA_READY:1;                
    Uint16 DATA_REQUEST:1;              
    Uint16 EOM:1;                       
    Uint16 NACK:1;                      
    Uint16 PEC_VALID:1;                 
    Uint16 CLK_LOW_TIMEOUT:1;           
    Uint16 CLK_HIGH_DETECTED:1;         
    Uint16 SLAVE_ADDR_READY:1;          
    Uint16 RPT_START:1;                 
    Uint16 UNIT_BUSY:1;                 
    Uint16 BUS_FREE:1;                  
    Uint16 LOST_ARB:1;                  
    Uint16 MASTER:1;                    
    Uint16 ALERT_EDGE:1;                
    Uint16 CONTROL_EDGE:1;              
    Uint16 ALERT_RAW:1;                 
    Uint16 CONTROL_RAW:1;               
    Uint16 SDA_RAW:1;                   
    Uint16 SCL_RAW:1;                   
    Uint16 rsvd1:10;                    
};

union PMBSTS_REG {
    Uint32  all;
    struct  PMBSTS_BITS  bit;
};

struct PMBINTM_BITS {                   
    Uint16 BUS_FREE:1;                  
    Uint16 BUS_LOW_TIMEOUT:1;           
    Uint16 DATA_READY:1;                
    Uint16 DATA_REQUEST:1;              
    Uint16 SLAVE_ADDR_READY:1;          
    Uint16 EOM:1;                       
    Uint16 ALERT:1;                     
    Uint16 CONTROL:1;                   
    Uint16 LOST_ARB:1;                  
    Uint16 CLK_HIGH_DETECT:1;           
    Uint16 rsvd1:6;                     
    Uint16 rsvd2:16;                    
};

union PMBINTM_REG {
    Uint32  all;
    struct  PMBINTM_BITS  bit;
};

struct PMBSC_BITS {                     
    Uint16 SLAVE_ADDR:7;                
    Uint16 MAN_SLAVE_ACK:1;             
    Uint16 SLAVE_MASK:7;                
    Uint16 PEC_ENA:1;                   
    Uint16 TX_COUNT:3;                  
    Uint16 TX_PEC:1;                    
    Uint16 MAN_CMD:1;                   
    Uint16 RX_BYTE_ACK_CNT:2;           
    Uint16 rsvd1:9;                     
};

union PMBSC_REG {
    Uint32  all;
    struct  PMBSC_BITS  bit;
};

struct PMBHSA_BITS {                    
    Uint16 SLAVE_RW:1;                  
    Uint16 SLAVE_ADDR:7;                
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union PMBHSA_REG {
    Uint32  all;
    struct  PMBHSA_BITS  bit;
};

struct PMBCTRL_BITS {                   
    Uint16 RESET:1;                     
    Uint16 ALERT_EN:1;                  
    Uint16 BUS_LO_INT_EDGE:1;           
    Uint16 FAST_MODE:1;                 
    Uint16 rsvd1:1;                     
    Uint16 CNTL_INT_EDGE:1;             
    Uint16 ALERT_MODE:1;                
    Uint16 ALERT_VALUE:1;               
    Uint16 ALERT_DIR:1;                 
    Uint16 CNTL_MODE:1;                 
    Uint16 CNTL_VALUE:1;                
    Uint16 CNTL_DIR:1;                  
    Uint16 SDA_MODE:1;                  
    Uint16 SDA_VALUE:1;                 
    Uint16 SDA_DIR:1;                   
    Uint16 SCL_MODE:1;                  
    Uint16 SCL_VALUE:1;                 
    Uint16 SCL_DIR:1;                   
    Uint16 IBIAS_A_EN:1;                
    Uint16 IBIAS_B_EN:1;                
    Uint16 CLK_LO_DIS:1;                
    Uint16 SLAVE_EN:1;                  
    Uint16 MASTER_EN:1;                 
    Uint16 CLKDIV:5;                    
    Uint16 rsvd2:3;                     
    Uint16 I2CMODE:1;                   
};

union PMBCTRL_REG {
    Uint32  all;
    struct  PMBCTRL_BITS  bit;
};

struct PMBTIMCTL_BITS {                 
    Uint16 TIM_OVERRIDE:1;              
    Uint16 rsvd1:15;                    
    Uint16 rsvd2:16;                    
};

union PMBTIMCTL_REG {
    Uint32  all;
    struct  PMBTIMCTL_BITS  bit;
};

struct PMBTIMCLK_BITS {                 
    Uint16 CLK_HIGH_LIMIT:8;            
    Uint16 rsvd1:8;                     
    Uint16 CLK_FREQ:8;                  
    Uint16 rsvd2:8;                     
};

union PMBTIMCLK_REG {
    Uint32  all;
    struct  PMBTIMCLK_BITS  bit;
};

struct PMBTIMSTSETUP_BITS {             
    Uint16 TSU_STA:8;                   
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union PMBTIMSTSETUP_REG {
    Uint32  all;
    struct  PMBTIMSTSETUP_BITS  bit;
};

struct PMBTIMBIDLE_BITS {               
    Uint16 BUSIDLE:10;                  
    Uint16 rsvd1:6;                     
    Uint16 rsvd2:16;                    
};

union PMBTIMBIDLE_REG {
    Uint32  all;
    struct  PMBTIMBIDLE_BITS  bit;
};

struct PMBTIMLOWTIMOUT_BITS {           
    Uint32 CLKLOWTIMOUT:20;             
    Uint16 rsvd1:12;                    
};

union PMBTIMLOWTIMOUT_REG {
    Uint32  all;
    struct  PMBTIMLOWTIMOUT_BITS  bit;
};

struct PMBTIMHIGHTIMOUT_BITS {          
    Uint16 CLKHIGHTIMOUT:10;            
    Uint16 rsvd1:6;                     
    Uint16 rsvd2:16;                    
};

union PMBTIMHIGHTIMOUT_REG {
    Uint32  all;
    struct  PMBTIMHIGHTIMOUT_BITS  bit;
};

struct PMBUS_REGS {
    union   PMBMC_REG                        PMBMC;                        
    Uint32                                   PMBTXBUF;                     
    Uint32                                   PMBRXBUF;                     
    union   PMBACK_REG                       PMBACK;                       
    union   PMBSTS_REG                       PMBSTS;                       
    union   PMBINTM_REG                      PMBINTM;                      
    union   PMBSC_REG                        PMBSC;                        
    union   PMBHSA_REG                       PMBHSA;                       
    union   PMBCTRL_REG                      PMBCTRL;                      
    union   PMBTIMCTL_REG                    PMBTIMCTL;                    
    union   PMBTIMCLK_REG                    PMBTIMCLK;                    
    union   PMBTIMSTSETUP_REG                PMBTIMSTSETUP;                
    union   PMBTIMBIDLE_REG                  PMBTIMBIDLE;                  
    union   PMBTIMLOWTIMOUT_REG              PMBTIMLOWTIMOUT;              
    union   PMBTIMHIGHTIMOUT_REG             PMBTIMHIGHTIMOUT;             
};




extern volatile struct PMBUS_REGS PmbusaRegs;



















































struct SCICCR_BITS {                    
    Uint16 SCICHAR:3;                   
    Uint16 ADDRIDLE_MODE:1;             
    Uint16 LOOPBKENA:1;                 
    Uint16 PARITYENA:1;                 
    Uint16 PARITY:1;                    
    Uint16 STOPBITS:1;                  
    Uint16 rsvd1:8;                     
};

union SCICCR_REG {
    Uint16  all;
    struct  SCICCR_BITS  bit;
};

struct SCICTL1_BITS {                   
    Uint16 RXENA:1;                     
    Uint16 TXENA:1;                     
    Uint16 SLEEP:1;                     
    Uint16 TXWAKE:1;                    
    Uint16 rsvd1:1;                     
    Uint16 SWRESET:1;                   
    Uint16 RXERRINTENA:1;               
    Uint16 rsvd2:9;                     
};

union SCICTL1_REG {
    Uint16  all;
    struct  SCICTL1_BITS  bit;
};

struct SCIHBAUD_BITS {                  
    Uint16 BAUD:8;                      
    Uint16 rsvd1:8;                     
};

union SCIHBAUD_REG {
    Uint16  all;
    struct  SCIHBAUD_BITS  bit;
};

struct SCILBAUD_BITS {                  
    Uint16 BAUD:8;                      
    Uint16 rsvd1:8;                     
};

union SCILBAUD_REG {
    Uint16  all;
    struct  SCILBAUD_BITS  bit;
};

struct SCICTL2_BITS {                   
    Uint16 TXINTENA:1;                  
    Uint16 RXBKINTENA:1;                
    Uint16 rsvd1:4;                     
    Uint16 TXEMPTY:1;                   
    Uint16 TXRDY:1;                     
    Uint16 rsvd2:8;                     
};

union SCICTL2_REG {
    Uint16  all;
    struct  SCICTL2_BITS  bit;
};

struct SCIRXST_BITS {                   
    Uint16 rsvd1:1;                     
    Uint16 RXWAKE:1;                    
    Uint16 PE:1;                        
    Uint16 OE:1;                        
    Uint16 FE:1;                        
    Uint16 BRKDT:1;                     
    Uint16 RXRDY:1;                     
    Uint16 RXERROR:1;                   
    Uint16 rsvd2:8;                     
};

union SCIRXST_REG {
    Uint16  all;
    struct  SCIRXST_BITS  bit;
};

struct SCIRXEMU_BITS {                  
    Uint16 ERXDT:8;                     
    Uint16 rsvd1:8;                     
};

union SCIRXEMU_REG {
    Uint16  all;
    struct  SCIRXEMU_BITS  bit;
};

struct SCIRXBUF_BITS {                  
    Uint16 SAR:8;                       
    Uint16 rsvd1:6;                     
    Uint16 SCIFFPE:1;                   
    Uint16 SCIFFFE:1;                   
};

union SCIRXBUF_REG {
    Uint16  all;
    struct  SCIRXBUF_BITS  bit;
};

struct SCITXBUF_BITS {                  
    Uint16 TXDT:8;                      
    Uint16 rsvd1:8;                     
};

union SCITXBUF_REG {
    Uint16  all;
    struct  SCITXBUF_BITS  bit;
};

struct SCIFFTX_BITS {                   
    Uint16 TXFFIL:5;                    
    Uint16 TXFFIENA:1;                  
    Uint16 TXFFINTCLR:1;                
    Uint16 TXFFINT:1;                   
    Uint16 TXFFST:5;                    
    Uint16 TXFIFORESET:1;               
    Uint16 SCIFFENA:1;                  
    Uint16 SCIRST:1;                    
};

union SCIFFTX_REG {
    Uint16  all;
    struct  SCIFFTX_BITS  bit;
};

struct SCIFFRX_BITS {                   
    Uint16 RXFFIL:5;                    
    Uint16 RXFFIENA:1;                  
    Uint16 RXFFINTCLR:1;                
    Uint16 RXFFINT:1;                   
    Uint16 RXFFST:5;                    
    Uint16 RXFIFORESET:1;               
    Uint16 RXFFOVRCLR:1;                
    Uint16 RXFFOVF:1;                   
};

union SCIFFRX_REG {
    Uint16  all;
    struct  SCIFFRX_BITS  bit;
};

struct SCIFFCT_BITS {                   
    Uint16 FFTXDLY:8;                   
    Uint16 rsvd1:5;                     
    Uint16 CDC:1;                       
    Uint16 ABDCLR:1;                    
    Uint16 ABD:1;                       
};

union SCIFFCT_REG {
    Uint16  all;
    struct  SCIFFCT_BITS  bit;
};

struct SCIPRI_BITS {                    
    Uint16 rsvd1:3;                     
    Uint16 FREESOFT:2;                  
    Uint16 rsvd2:3;                     
    Uint16 rsvd3:8;                     
};

union SCIPRI_REG {
    Uint16  all;
    struct  SCIPRI_BITS  bit;
};

struct SCI_REGS {
    union   SCICCR_REG                       SCICCR;                       
    union   SCICTL1_REG                      SCICTL1;                      
    union   SCIHBAUD_REG                     SCIHBAUD;                     
    union   SCILBAUD_REG                     SCILBAUD;                     
    union   SCICTL2_REG                      SCICTL2;                      
    union   SCIRXST_REG                      SCIRXST;                      
    union   SCIRXEMU_REG                     SCIRXEMU;                     
    union   SCIRXBUF_REG                     SCIRXBUF;                     
    Uint16                                   rsvd1;                        
    union   SCITXBUF_REG                     SCITXBUF;                     
    union   SCIFFTX_REG                      SCIFFTX;                      
    union   SCIFFRX_REG                      SCIFFRX;                      
    union   SCIFFCT_REG                      SCIFFCT;                      
    Uint16                                   rsvd2[2];                     
    union   SCIPRI_REG                       SCIPRI;                       
};




extern volatile struct SCI_REGS SciaRegs;
extern volatile struct SCI_REGS ScibRegs;



















































struct SDIFLG_BITS {                    
    Uint16 FLT1_FLG_CEVT1:1;            
    Uint16 FLT1_FLG_CEVT2:1;            
    Uint16 FLT2_FLG_CEVT1:1;            
    Uint16 FLT2_FLG_CEVT2:1;            
    Uint16 FLT3_FLG_CEVT1:1;            
    Uint16 FLT3_FLG_CEVT2:1;            
    Uint16 FLT4_FLG_CEVT1:1;            
    Uint16 FLT4_FLG_CEVT2:1;            
    Uint16 MF1:1;                       
    Uint16 MF2:1;                       
    Uint16 MF3:1;                       
    Uint16 MF4:1;                       
    Uint16 AF1:1;                       
    Uint16 AF2:1;                       
    Uint16 AF3:1;                       
    Uint16 AF4:1;                       
    Uint16 SDFFOVF1:1;                  
    Uint16 SDFFOVF2:1;                  
    Uint16 SDFFOVF3:1;                  
    Uint16 SDFFOVF4:1;                  
    Uint16 SDFFINT1:1;                  
    Uint16 SDFFINT2:1;                  
    Uint16 SDFFINT3:1;                  
    Uint16 SDFFINT4:1;                  
    Uint16 rsvd1:7;                     
    Uint16 MIF:1;                       
};

union SDIFLG_REG {
    Uint32  all;
    struct  SDIFLG_BITS  bit;
};

struct SDIFLGCLR_BITS {                 
    Uint16 FLT1_FLG_CEVT1:1;            
    Uint16 FLT1_FLG_CEVT2:1;            
    Uint16 FLT2_FLG_CEVT1:1;            
    Uint16 FLT2_FLG_CEVT2:1;            
    Uint16 FLT3_FLG_CEVT1:1;            
    Uint16 FLT3_FLG_CEVT2:1;            
    Uint16 FLT4_FLG_CEVT1:1;            
    Uint16 FLT4_FLG_CEVT2:1;            
    Uint16 MF1:1;                       
    Uint16 MF2:1;                       
    Uint16 MF3:1;                       
    Uint16 MF4:1;                       
    Uint16 AF1:1;                       
    Uint16 AF2:1;                       
    Uint16 AF3:1;                       
    Uint16 AF4:1;                       
    Uint16 SDFFOVF1:1;                  
    Uint16 SDFFOVF2:1;                  
    Uint16 SDFFOVF3:1;                  
    Uint16 SDFFOVF4:1;                  
    Uint16 SDFFINT1:1;                  
    Uint16 SDFFINT2:1;                  
    Uint16 SDFFINT3:1;                  
    Uint16 SDFFINT4:1;                  
    Uint16 rsvd1:7;                     
    Uint16 MIF:1;                       
};

union SDIFLGCLR_REG {
    Uint32  all;
    struct  SDIFLGCLR_BITS  bit;
};

struct SDCTL_BITS {                     
    Uint16 HZ1:1;                       
    Uint16 HZ2:1;                       
    Uint16 HZ3:1;                       
    Uint16 HZ4:1;                       
    Uint16 rsvd1:9;                     
    Uint16 MIE:1;                       
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
};

union SDCTL_REG {
    Uint16  all;
    struct  SDCTL_BITS  bit;
};

struct SDMFILEN_BITS {                  
    Uint16 rsvd1:4;                     
    Uint16 rsvd2:3;                     
    Uint16 rsvd3:2;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 MFE:1;                       
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:3;                     
};

union SDMFILEN_REG {
    Uint16  all;
    struct  SDMFILEN_BITS  bit;
};

struct SDSTATUS_BITS {                  
    Uint16 HZ1:1;                       
    Uint16 HZ2:1;                       
    Uint16 HZ3:1;                       
    Uint16 HZ4:1;                       
    Uint16 rsvd1:4;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:1;                     
    Uint16 rsvd8:1;                     
    Uint16 rsvd9:1;                     
};

union SDSTATUS_REG {
    Uint16  all;
    struct  SDSTATUS_BITS  bit;
};

struct SDCTLPARM1_BITS {                
    Uint16 MOD:2;                       
    Uint16 rsvd1:1;                     
    Uint16 SDCLKSEL:1;                  
    Uint16 SDCLKSYNC:1;                 
    Uint16 rsvd2:1;                     
    Uint16 SDDATASYNC:1;                
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:8;                     
};

union SDCTLPARM1_REG {
    Uint16  all;
    struct  SDCTLPARM1_BITS  bit;
};

struct SDDFPARM1_BITS {                 
    Uint16 DOSR:8;                      
    Uint16 FEN:1;                       
    Uint16 AE:1;                        
    Uint16 SST:2;                       
    Uint16 SDSYNCEN:1;                  
    Uint16 rsvd1:3;                     
};

union SDDFPARM1_REG {
    Uint16  all;
    struct  SDDFPARM1_BITS  bit;
};

struct SDDPARM1_BITS {                  
    Uint16 rsvd1:10;                    
    Uint16 DR:1;                        
    Uint16 SH:5;                        
};

union SDDPARM1_REG {
    Uint16  all;
    struct  SDDPARM1_BITS  bit;
};

struct SDFLT1CMPH1_BITS {               
    Uint16 HLT:15;                      
    Uint16 rsvd1:1;                     
};

union SDFLT1CMPH1_REG {
    Uint16  all;
    struct  SDFLT1CMPH1_BITS  bit;
};

struct SDFLT1CMPL1_BITS {               
    Uint16 LLT:15;                      
    Uint16 rsvd1:1;                     
};

union SDFLT1CMPL1_REG {
    Uint16  all;
    struct  SDFLT1CMPL1_BITS  bit;
};

struct SDCPARM1_BITS {                  
    Uint16 COSR:5;                      
    Uint16 EN_CEVT1:1;                  
    Uint16 EN_CEVT2:1;                  
    Uint16 CS1_CS0:2;                   
    Uint16 MFIE:1;                      
    Uint16 HZEN:1;                      
    Uint16 CEVT1SEL:2;                  
    Uint16 CEN:1;                       
    Uint16 CEVT2SEL:2;                  
};

union SDCPARM1_REG {
    Uint16  all;
    struct  SDCPARM1_BITS  bit;
};

struct SDDATA1_BITS {                   
    Uint16 DATA16:16;                   
    Uint16 DATA32HI:16;                 
};

union SDDATA1_REG {
    Uint32  all;
    struct  SDDATA1_BITS  bit;
};

struct SDDATFIFO1_BITS {                
    Uint16 DATA16:16;                   
    Uint16 DATA32HI:16;                 
};

union SDDATFIFO1_REG {
    Uint32  all;
    struct  SDDATFIFO1_BITS  bit;
};

struct SDFLT1CMPH2_BITS {               
    Uint16 HLT2:15;                     
    Uint16 rsvd1:1;                     
};

union SDFLT1CMPH2_REG {
    Uint16  all;
    struct  SDFLT1CMPH2_BITS  bit;
};

struct SDFLT1CMPHZ_BITS {               
    Uint16 HLTZ:15;                     
    Uint16 rsvd1:1;                     
};

union SDFLT1CMPHZ_REG {
    Uint16  all;
    struct  SDFLT1CMPHZ_BITS  bit;
};

struct SDFIFOCTL1_BITS {                
    Uint16 SDFFIL:5;                    
    Uint16 rsvd1:1;                     
    Uint16 SDFFST:5;                    
    Uint16 rsvd2:1;                     
    Uint16 FFIEN:1;                     
    Uint16 FFEN:1;                      
    Uint16 DRINTSEL:1;                  
    Uint16 OVFIEN:1;                    
};

union SDFIFOCTL1_REG {
    Uint16  all;
    struct  SDFIFOCTL1_BITS  bit;
};

struct SDSYNC1_BITS {                   
    Uint16 SYNCSEL:6;                   
    Uint16 WTSYNCEN:1;                  
    Uint16 WTSYNFLG:1;                  
    Uint16 WTSYNCLR:1;                  
    Uint16 FFSYNCCLREN:1;               
    Uint16 WTSCLREN:1;                  
    Uint16 rsvd1:5;                     
};

union SDSYNC1_REG {
    Uint16  all;
    struct  SDSYNC1_BITS  bit;
};

struct SDFLT1CMPL2_BITS {               
    Uint16 LLT2:15;                     
    Uint16 rsvd1:1;                     
};

union SDFLT1CMPL2_REG {
    Uint16  all;
    struct  SDFLT1CMPL2_BITS  bit;
};

struct SDCTLPARM2_BITS {                
    Uint16 MOD:2;                       
    Uint16 rsvd1:1;                     
    Uint16 SDCLKSEL:1;                  
    Uint16 SDCLKSYNC:1;                 
    Uint16 rsvd2:1;                     
    Uint16 SDDATASYNC:1;                
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:8;                     
};

union SDCTLPARM2_REG {
    Uint16  all;
    struct  SDCTLPARM2_BITS  bit;
};

struct SDDFPARM2_BITS {                 
    Uint16 DOSR:8;                      
    Uint16 FEN:1;                       
    Uint16 AE:1;                        
    Uint16 SST:2;                       
    Uint16 SDSYNCEN:1;                  
    Uint16 rsvd1:3;                     
};

union SDDFPARM2_REG {
    Uint16  all;
    struct  SDDFPARM2_BITS  bit;
};

struct SDDPARM2_BITS {                  
    Uint16 rsvd1:10;                    
    Uint16 DR:1;                        
    Uint16 SH:5;                        
};

union SDDPARM2_REG {
    Uint16  all;
    struct  SDDPARM2_BITS  bit;
};

struct SDFLT2CMPH1_BITS {               
    Uint16 HLT:15;                      
    Uint16 rsvd1:1;                     
};

union SDFLT2CMPH1_REG {
    Uint16  all;
    struct  SDFLT2CMPH1_BITS  bit;
};

struct SDFLT2CMPL1_BITS {               
    Uint16 LLT:15;                      
    Uint16 rsvd1:1;                     
};

union SDFLT2CMPL1_REG {
    Uint16  all;
    struct  SDFLT2CMPL1_BITS  bit;
};

struct SDCPARM2_BITS {                  
    Uint16 COSR:5;                      
    Uint16 EN_CEVT1:1;                  
    Uint16 EN_CEVT2:1;                  
    Uint16 CS1_CS0:2;                   
    Uint16 MFIE:1;                      
    Uint16 HZEN:1;                      
    Uint16 CEVT1SEL:2;                  
    Uint16 CEN:1;                       
    Uint16 CEVT2SEL:2;                  
};

union SDCPARM2_REG {
    Uint16  all;
    struct  SDCPARM2_BITS  bit;
};

struct SDDATA2_BITS {                   
    Uint16 DATA16:16;                   
    Uint16 DATA32HI:16;                 
};

union SDDATA2_REG {
    Uint32  all;
    struct  SDDATA2_BITS  bit;
};

struct SDDATFIFO2_BITS {                
    Uint16 DATA16:16;                   
    Uint16 DATA32HI:16;                 
};

union SDDATFIFO2_REG {
    Uint32  all;
    struct  SDDATFIFO2_BITS  bit;
};

struct SDFLT2CMPH2_BITS {               
    Uint16 HLT2:15;                     
    Uint16 rsvd1:1;                     
};

union SDFLT2CMPH2_REG {
    Uint16  all;
    struct  SDFLT2CMPH2_BITS  bit;
};

struct SDFLT2CMPHZ_BITS {               
    Uint16 HLTZ:15;                     
    Uint16 rsvd1:1;                     
};

union SDFLT2CMPHZ_REG {
    Uint16  all;
    struct  SDFLT2CMPHZ_BITS  bit;
};

struct SDFIFOCTL2_BITS {                
    Uint16 SDFFIL:5;                    
    Uint16 rsvd1:1;                     
    Uint16 SDFFST:5;                    
    Uint16 rsvd2:1;                     
    Uint16 FFIEN:1;                     
    Uint16 FFEN:1;                      
    Uint16 DRINTSEL:1;                  
    Uint16 OVFIEN:1;                    
};

union SDFIFOCTL2_REG {
    Uint16  all;
    struct  SDFIFOCTL2_BITS  bit;
};

struct SDSYNC2_BITS {                   
    Uint16 SYNCSEL:6;                   
    Uint16 WTSYNCEN:1;                  
    Uint16 WTSYNFLG:1;                  
    Uint16 WTSYNCLR:1;                  
    Uint16 FFSYNCCLREN:1;               
    Uint16 WTSCLREN:1;                  
    Uint16 rsvd1:5;                     
};

union SDSYNC2_REG {
    Uint16  all;
    struct  SDSYNC2_BITS  bit;
};

struct SDFLT2CMPL2_BITS {               
    Uint16 LLT2:15;                     
    Uint16 rsvd1:1;                     
};

union SDFLT2CMPL2_REG {
    Uint16  all;
    struct  SDFLT2CMPL2_BITS  bit;
};

struct SDCTLPARM3_BITS {                
    Uint16 MOD:2;                       
    Uint16 rsvd1:1;                     
    Uint16 SDCLKSEL:1;                  
    Uint16 SDCLKSYNC:1;                 
    Uint16 rsvd2:1;                     
    Uint16 SDDATASYNC:1;                
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:8;                     
};

union SDCTLPARM3_REG {
    Uint16  all;
    struct  SDCTLPARM3_BITS  bit;
};

struct SDDFPARM3_BITS {                 
    Uint16 DOSR:8;                      
    Uint16 FEN:1;                       
    Uint16 AE:1;                        
    Uint16 SST:2;                       
    Uint16 SDSYNCEN:1;                  
    Uint16 rsvd1:3;                     
};

union SDDFPARM3_REG {
    Uint16  all;
    struct  SDDFPARM3_BITS  bit;
};

struct SDDPARM3_BITS {                  
    Uint16 rsvd1:10;                    
    Uint16 DR:1;                        
    Uint16 SH:5;                        
};

union SDDPARM3_REG {
    Uint16  all;
    struct  SDDPARM3_BITS  bit;
};

struct SDFLT3CMPH1_BITS {               
    Uint16 HLT:15;                      
    Uint16 rsvd1:1;                     
};

union SDFLT3CMPH1_REG {
    Uint16  all;
    struct  SDFLT3CMPH1_BITS  bit;
};

struct SDFLT3CMPL1_BITS {               
    Uint16 LLT:15;                      
    Uint16 rsvd1:1;                     
};

union SDFLT3CMPL1_REG {
    Uint16  all;
    struct  SDFLT3CMPL1_BITS  bit;
};

struct SDCPARM3_BITS {                  
    Uint16 COSR:5;                      
    Uint16 EN_CEVT1:1;                  
    Uint16 EN_CEVT2:1;                  
    Uint16 CS1_CS0:2;                   
    Uint16 MFIE:1;                      
    Uint16 HZEN:1;                      
    Uint16 CEVT1SEL:2;                  
    Uint16 CEN:1;                       
    Uint16 CEVT2SEL:2;                  
};

union SDCPARM3_REG {
    Uint16  all;
    struct  SDCPARM3_BITS  bit;
};

struct SDDATA3_BITS {                   
    Uint16 DATA16:16;                   
    Uint16 DATA32HI:16;                 
};

union SDDATA3_REG {
    Uint32  all;
    struct  SDDATA3_BITS  bit;
};

struct SDDATFIFO3_BITS {                
    Uint16 DATA16:16;                   
    Uint16 DATA32HI:16;                 
};

union SDDATFIFO3_REG {
    Uint32  all;
    struct  SDDATFIFO3_BITS  bit;
};

struct SDFLT3CMPH2_BITS {               
    Uint16 HLT2:15;                     
    Uint16 rsvd1:1;                     
};

union SDFLT3CMPH2_REG {
    Uint16  all;
    struct  SDFLT3CMPH2_BITS  bit;
};

struct SDFLT3CMPHZ_BITS {               
    Uint16 HLTZ:15;                     
    Uint16 rsvd1:1;                     
};

union SDFLT3CMPHZ_REG {
    Uint16  all;
    struct  SDFLT3CMPHZ_BITS  bit;
};

struct SDFIFOCTL3_BITS {                
    Uint16 SDFFIL:5;                    
    Uint16 rsvd1:1;                     
    Uint16 SDFFST:5;                    
    Uint16 rsvd2:1;                     
    Uint16 FFIEN:1;                     
    Uint16 FFEN:1;                      
    Uint16 DRINTSEL:1;                  
    Uint16 OVFIEN:1;                    
};

union SDFIFOCTL3_REG {
    Uint16  all;
    struct  SDFIFOCTL3_BITS  bit;
};

struct SDSYNC3_BITS {                   
    Uint16 SYNCSEL:6;                   
    Uint16 WTSYNCEN:1;                  
    Uint16 WTSYNFLG:1;                  
    Uint16 WTSYNCLR:1;                  
    Uint16 FFSYNCCLREN:1;               
    Uint16 WTSCLREN:1;                  
    Uint16 rsvd1:5;                     
};

union SDSYNC3_REG {
    Uint16  all;
    struct  SDSYNC3_BITS  bit;
};

struct SDFLT3CMPL2_BITS {               
    Uint16 LLT2:15;                     
    Uint16 rsvd1:1;                     
};

union SDFLT3CMPL2_REG {
    Uint16  all;
    struct  SDFLT3CMPL2_BITS  bit;
};

struct SDCTLPARM4_BITS {                
    Uint16 MOD:2;                       
    Uint16 rsvd1:1;                     
    Uint16 SDCLKSEL:1;                  
    Uint16 SDCLKSYNC:1;                 
    Uint16 rsvd2:1;                     
    Uint16 SDDATASYNC:1;                
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:8;                     
};

union SDCTLPARM4_REG {
    Uint16  all;
    struct  SDCTLPARM4_BITS  bit;
};

struct SDDFPARM4_BITS {                 
    Uint16 DOSR:8;                      
    Uint16 FEN:1;                       
    Uint16 AE:1;                        
    Uint16 SST:2;                       
    Uint16 SDSYNCEN:1;                  
    Uint16 rsvd1:3;                     
};

union SDDFPARM4_REG {
    Uint16  all;
    struct  SDDFPARM4_BITS  bit;
};

struct SDDPARM4_BITS {                  
    Uint16 rsvd1:10;                    
    Uint16 DR:1;                        
    Uint16 SH:5;                        
};

union SDDPARM4_REG {
    Uint16  all;
    struct  SDDPARM4_BITS  bit;
};

struct SDFLT4CMPH1_BITS {               
    Uint16 HLT:15;                      
    Uint16 rsvd1:1;                     
};

union SDFLT4CMPH1_REG {
    Uint16  all;
    struct  SDFLT4CMPH1_BITS  bit;
};

struct SDFLT4CMPL1_BITS {               
    Uint16 LLT:15;                      
    Uint16 rsvd1:1;                     
};

union SDFLT4CMPL1_REG {
    Uint16  all;
    struct  SDFLT4CMPL1_BITS  bit;
};

struct SDCPARM4_BITS {                  
    Uint16 COSR:5;                      
    Uint16 EN_CEVT1:1;                  
    Uint16 EN_CEVT2:1;                  
    Uint16 CS1_CS0:2;                   
    Uint16 MFIE:1;                      
    Uint16 HZEN:1;                      
    Uint16 CEVT1SEL:2;                  
    Uint16 CEN:1;                       
    Uint16 CEVT2SEL:2;                  
};

union SDCPARM4_REG {
    Uint16  all;
    struct  SDCPARM4_BITS  bit;
};

struct SDDATA4_BITS {                   
    Uint16 DATA16:16;                   
    Uint16 DATA32HI:16;                 
};

union SDDATA4_REG {
    Uint32  all;
    struct  SDDATA4_BITS  bit;
};

struct SDDATFIFO4_BITS {                
    Uint16 DATA16:16;                   
    Uint16 DATA32HI:16;                 
};

union SDDATFIFO4_REG {
    Uint32  all;
    struct  SDDATFIFO4_BITS  bit;
};

struct SDFLT4CMPH2_BITS {               
    Uint16 HLT2:15;                     
    Uint16 rsvd1:1;                     
};

union SDFLT4CMPH2_REG {
    Uint16  all;
    struct  SDFLT4CMPH2_BITS  bit;
};

struct SDFLT4CMPHZ_BITS {               
    Uint16 HLTZ:15;                     
    Uint16 rsvd1:1;                     
};

union SDFLT4CMPHZ_REG {
    Uint16  all;
    struct  SDFLT4CMPHZ_BITS  bit;
};

struct SDFIFOCTL4_BITS {                
    Uint16 SDFFIL:5;                    
    Uint16 rsvd1:1;                     
    Uint16 SDFFST:5;                    
    Uint16 rsvd2:1;                     
    Uint16 FFIEN:1;                     
    Uint16 FFEN:1;                      
    Uint16 DRINTSEL:1;                  
    Uint16 OVFIEN:1;                    
};

union SDFIFOCTL4_REG {
    Uint16  all;
    struct  SDFIFOCTL4_BITS  bit;
};

struct SDSYNC4_BITS {                   
    Uint16 SYNCSEL:6;                   
    Uint16 WTSYNCEN:1;                  
    Uint16 WTSYNFLG:1;                  
    Uint16 WTSYNCLR:1;                  
    Uint16 FFSYNCCLREN:1;               
    Uint16 WTSCLREN:1;                  
    Uint16 rsvd1:5;                     
};

union SDSYNC4_REG {
    Uint16  all;
    struct  SDSYNC4_BITS  bit;
};

struct SDFLT4CMPL2_BITS {               
    Uint16 LLT2:15;                     
    Uint16 rsvd1:1;                     
};

union SDFLT4CMPL2_REG {
    Uint16  all;
    struct  SDFLT4CMPL2_BITS  bit;
};

struct SDCOMP1CTL_BITS {                
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 CEVT1DIGFILTSEL:2;           
    Uint16 rsvd3:2;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:1;                     
    Uint16 CEVT2DIGFILTSEL:2;           
    Uint16 rsvd8:2;                     
    Uint16 rsvd9:1;                     
    Uint16 rsvd10:1;                    
};

union SDCOMP1CTL_REG {
    Uint16  all;
    struct  SDCOMP1CTL_BITS  bit;
};

struct SDCOMP1EVT2FLTCTL_BITS {         
    Uint16 rsvd1:4;                     
    Uint16 SAMPWIN:5;                   
    Uint16 THRESH:5;                    
    Uint16 rsvd2:1;                     
    Uint16 FILINIT:1;                   
};

union SDCOMP1EVT2FLTCTL_REG {
    Uint16  all;
    struct  SDCOMP1EVT2FLTCTL_BITS  bit;
};

struct SDCOMP1EVT2FLTCLKCTL_BITS {      
    Uint16 CLKPRESCALE:10;              
    Uint16 rsvd1:6;                     
};

union SDCOMP1EVT2FLTCLKCTL_REG {
    Uint16  all;
    struct  SDCOMP1EVT2FLTCLKCTL_BITS  bit;
};

struct SDCOMP1EVT1FLTCTL_BITS {         
    Uint16 rsvd1:4;                     
    Uint16 SAMPWIN:5;                   
    Uint16 THRESH:5;                    
    Uint16 rsvd2:1;                     
    Uint16 FILINIT:1;                   
};

union SDCOMP1EVT1FLTCTL_REG {
    Uint16  all;
    struct  SDCOMP1EVT1FLTCTL_BITS  bit;
};

struct SDCOMP1EVT1FLTCLKCTL_BITS {      
    Uint16 CLKPRESCALE:10;              
    Uint16 rsvd1:6;                     
};

union SDCOMP1EVT1FLTCLKCTL_REG {
    Uint16  all;
    struct  SDCOMP1EVT1FLTCLKCTL_BITS  bit;
};

struct SDCOMP1LOCK_BITS {               
    Uint16 SDCOMP1CTL:1;                
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 COMP:1;                      
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:11;                    
};

union SDCOMP1LOCK_REG {
    Uint16  all;
    struct  SDCOMP1LOCK_BITS  bit;
};

struct SDCOMP2CTL_BITS {                
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 CEVT1DIGFILTSEL:2;           
    Uint16 rsvd3:2;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:1;                     
    Uint16 CEVT2DIGFILTSEL:2;           
    Uint16 rsvd8:2;                     
    Uint16 rsvd9:1;                     
    Uint16 rsvd10:1;                    
};

union SDCOMP2CTL_REG {
    Uint16  all;
    struct  SDCOMP2CTL_BITS  bit;
};

struct SDCOMP2EVT2FLTCTL_BITS {         
    Uint16 rsvd1:4;                     
    Uint16 SAMPWIN:5;                   
    Uint16 THRESH:5;                    
    Uint16 rsvd2:1;                     
    Uint16 FILINIT:1;                   
};

union SDCOMP2EVT2FLTCTL_REG {
    Uint16  all;
    struct  SDCOMP2EVT2FLTCTL_BITS  bit;
};

struct SDCOMP2EVT2FLTCLKCTL_BITS {      
    Uint16 CLKPRESCALE:10;              
    Uint16 rsvd1:6;                     
};

union SDCOMP2EVT2FLTCLKCTL_REG {
    Uint16  all;
    struct  SDCOMP2EVT2FLTCLKCTL_BITS  bit;
};

struct SDCOMP2EVT1FLTCTL_BITS {         
    Uint16 rsvd1:4;                     
    Uint16 SAMPWIN:5;                   
    Uint16 THRESH:5;                    
    Uint16 rsvd2:1;                     
    Uint16 FILINIT:1;                   
};

union SDCOMP2EVT1FLTCTL_REG {
    Uint16  all;
    struct  SDCOMP2EVT1FLTCTL_BITS  bit;
};

struct SDCOMP2EVT1FLTCLKCTL_BITS {      
    Uint16 CLKPRESCALE:10;              
    Uint16 rsvd1:6;                     
};

union SDCOMP2EVT1FLTCLKCTL_REG {
    Uint16  all;
    struct  SDCOMP2EVT1FLTCLKCTL_BITS  bit;
};

struct SDCOMP2LOCK_BITS {               
    Uint16 SDCOMP2CTL:1;                
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 COMP:1;                      
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:11;                    
};

union SDCOMP2LOCK_REG {
    Uint16  all;
    struct  SDCOMP2LOCK_BITS  bit;
};

struct SDCOMP3CTL_BITS {                
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 CEVT1DIGFILTSEL:2;           
    Uint16 rsvd3:2;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:1;                     
    Uint16 CEVT2DIGFILTSEL:2;           
    Uint16 rsvd8:2;                     
    Uint16 rsvd9:1;                     
    Uint16 rsvd10:1;                    
};

union SDCOMP3CTL_REG {
    Uint16  all;
    struct  SDCOMP3CTL_BITS  bit;
};

struct SDCOMP3EVT2FLTCTL_BITS {         
    Uint16 rsvd1:4;                     
    Uint16 SAMPWIN:5;                   
    Uint16 THRESH:5;                    
    Uint16 rsvd2:1;                     
    Uint16 FILINIT:1;                   
};

union SDCOMP3EVT2FLTCTL_REG {
    Uint16  all;
    struct  SDCOMP3EVT2FLTCTL_BITS  bit;
};

struct SDCOMP3EVT2FLTCLKCTL_BITS {      
    Uint16 CLKPRESCALE:10;              
    Uint16 rsvd1:6;                     
};

union SDCOMP3EVT2FLTCLKCTL_REG {
    Uint16  all;
    struct  SDCOMP3EVT2FLTCLKCTL_BITS  bit;
};

struct SDCOMP3EVT1FLTCTL_BITS {         
    Uint16 rsvd1:4;                     
    Uint16 SAMPWIN:5;                   
    Uint16 THRESH:5;                    
    Uint16 rsvd2:1;                     
    Uint16 FILINIT:1;                   
};

union SDCOMP3EVT1FLTCTL_REG {
    Uint16  all;
    struct  SDCOMP3EVT1FLTCTL_BITS  bit;
};

struct SDCOMP3EVT1FLTCLKCTL_BITS {      
    Uint16 CLKPRESCALE:10;              
    Uint16 rsvd1:6;                     
};

union SDCOMP3EVT1FLTCLKCTL_REG {
    Uint16  all;
    struct  SDCOMP3EVT1FLTCLKCTL_BITS  bit;
};

struct SDCOMP3LOCK_BITS {               
    Uint16 SDCOMP3CTL:1;                
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 COMP:1;                      
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:11;                    
};

union SDCOMP3LOCK_REG {
    Uint16  all;
    struct  SDCOMP3LOCK_BITS  bit;
};

struct SDCOMP4CTL_BITS {                
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 CEVT1DIGFILTSEL:2;           
    Uint16 rsvd3:2;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:1;                     
    Uint16 CEVT2DIGFILTSEL:2;           
    Uint16 rsvd8:2;                     
    Uint16 rsvd9:1;                     
    Uint16 rsvd10:1;                    
};

union SDCOMP4CTL_REG {
    Uint16  all;
    struct  SDCOMP4CTL_BITS  bit;
};

struct SDCOMP4EVT2FLTCTL_BITS {         
    Uint16 rsvd1:4;                     
    Uint16 SAMPWIN:5;                   
    Uint16 THRESH:5;                    
    Uint16 rsvd2:1;                     
    Uint16 FILINIT:1;                   
};

union SDCOMP4EVT2FLTCTL_REG {
    Uint16  all;
    struct  SDCOMP4EVT2FLTCTL_BITS  bit;
};

struct SDCOMP4EVT2FLTCLKCTL_BITS {      
    Uint16 CLKPRESCALE:10;              
    Uint16 rsvd1:6;                     
};

union SDCOMP4EVT2FLTCLKCTL_REG {
    Uint16  all;
    struct  SDCOMP4EVT2FLTCLKCTL_BITS  bit;
};

struct SDCOMP4EVT1FLTCTL_BITS {         
    Uint16 rsvd1:4;                     
    Uint16 SAMPWIN:5;                   
    Uint16 THRESH:5;                    
    Uint16 rsvd2:1;                     
    Uint16 FILINIT:1;                   
};

union SDCOMP4EVT1FLTCTL_REG {
    Uint16  all;
    struct  SDCOMP4EVT1FLTCTL_BITS  bit;
};

struct SDCOMP4EVT1FLTCLKCTL_BITS {      
    Uint16 CLKPRESCALE:10;              
    Uint16 rsvd1:6;                     
};

union SDCOMP4EVT1FLTCLKCTL_REG {
    Uint16  all;
    struct  SDCOMP4EVT1FLTCLKCTL_BITS  bit;
};

struct SDCOMP4LOCK_BITS {               
    Uint16 SDCOMP4CTL:1;                
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 COMP:1;                      
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:11;                    
};

union SDCOMP4LOCK_REG {
    Uint16  all;
    struct  SDCOMP4LOCK_BITS  bit;
};

struct SDFM_REGS {
    union   SDIFLG_REG                       SDIFLG;                       
    union   SDIFLGCLR_REG                    SDIFLGCLR;                    
    union   SDCTL_REG                        SDCTL;                        
    Uint16                                   rsvd1;                        
    union   SDMFILEN_REG                     SDMFILEN;                     
    union   SDSTATUS_REG                     SDSTATUS;                     
    Uint16                                   rsvd2[8];                     
    union   SDCTLPARM1_REG                   SDCTLPARM1;                   
    union   SDDFPARM1_REG                    SDDFPARM1;                    
    union   SDDPARM1_REG                     SDDPARM1;                     
    union   SDFLT1CMPH1_REG                  SDFLT1CMPH1;                  
    union   SDFLT1CMPL1_REG                  SDFLT1CMPL1;                  
    union   SDCPARM1_REG                     SDCPARM1;                     
    union   SDDATA1_REG                      SDDATA1;                      
    union   SDDATFIFO1_REG                   SDDATFIFO1;                   
    Uint16                                   SDCDATA1;                     
    union   SDFLT1CMPH2_REG                  SDFLT1CMPH2;                  
    union   SDFLT1CMPHZ_REG                  SDFLT1CMPHZ;                  
    union   SDFIFOCTL1_REG                   SDFIFOCTL1;                   
    union   SDSYNC1_REG                      SDSYNC1;                      
    union   SDFLT1CMPL2_REG                  SDFLT1CMPL2;                  
    union   SDCTLPARM2_REG                   SDCTLPARM2;                   
    union   SDDFPARM2_REG                    SDDFPARM2;                    
    union   SDDPARM2_REG                     SDDPARM2;                     
    union   SDFLT2CMPH1_REG                  SDFLT2CMPH1;                  
    union   SDFLT2CMPL1_REG                  SDFLT2CMPL1;                  
    union   SDCPARM2_REG                     SDCPARM2;                     
    union   SDDATA2_REG                      SDDATA2;                      
    union   SDDATFIFO2_REG                   SDDATFIFO2;                   
    Uint16                                   SDCDATA2;                     
    union   SDFLT2CMPH2_REG                  SDFLT2CMPH2;                  
    union   SDFLT2CMPHZ_REG                  SDFLT2CMPHZ;                  
    union   SDFIFOCTL2_REG                   SDFIFOCTL2;                   
    union   SDSYNC2_REG                      SDSYNC2;                      
    union   SDFLT2CMPL2_REG                  SDFLT2CMPL2;                  
    union   SDCTLPARM3_REG                   SDCTLPARM3;                   
    union   SDDFPARM3_REG                    SDDFPARM3;                    
    union   SDDPARM3_REG                     SDDPARM3;                     
    union   SDFLT3CMPH1_REG                  SDFLT3CMPH1;                  
    union   SDFLT3CMPL1_REG                  SDFLT3CMPL1;                  
    union   SDCPARM3_REG                     SDCPARM3;                     
    union   SDDATA3_REG                      SDDATA3;                      
    union   SDDATFIFO3_REG                   SDDATFIFO3;                   
    Uint16                                   SDCDATA3;                     
    union   SDFLT3CMPH2_REG                  SDFLT3CMPH2;                  
    union   SDFLT3CMPHZ_REG                  SDFLT3CMPHZ;                  
    union   SDFIFOCTL3_REG                   SDFIFOCTL3;                   
    union   SDSYNC3_REG                      SDSYNC3;                      
    union   SDFLT3CMPL2_REG                  SDFLT3CMPL2;                  
    union   SDCTLPARM4_REG                   SDCTLPARM4;                   
    union   SDDFPARM4_REG                    SDDFPARM4;                    
    union   SDDPARM4_REG                     SDDPARM4;                     
    union   SDFLT4CMPH1_REG                  SDFLT4CMPH1;                  
    union   SDFLT4CMPL1_REG                  SDFLT4CMPL1;                  
    union   SDCPARM4_REG                     SDCPARM4;                     
    union   SDDATA4_REG                      SDDATA4;                      
    union   SDDATFIFO4_REG                   SDDATFIFO4;                   
    Uint16                                   SDCDATA4;                     
    union   SDFLT4CMPH2_REG                  SDFLT4CMPH2;                  
    union   SDFLT4CMPHZ_REG                  SDFLT4CMPHZ;                  
    union   SDFIFOCTL4_REG                   SDFIFOCTL4;                   
    union   SDSYNC4_REG                      SDSYNC4;                      
    union   SDFLT4CMPL2_REG                  SDFLT4CMPL2;                  
    Uint16                                   rsvd3[16];                    
    union   SDCOMP1CTL_REG                   SDCOMP1CTL;                   
    union   SDCOMP1EVT2FLTCTL_REG            SDCOMP1EVT2FLTCTL;            
    union   SDCOMP1EVT2FLTCLKCTL_REG         SDCOMP1EVT2FLTCLKCTL;         
    union   SDCOMP1EVT1FLTCTL_REG            SDCOMP1EVT1FLTCTL;            
    union   SDCOMP1EVT1FLTCLKCTL_REG         SDCOMP1EVT1FLTCLKCTL;         
    Uint16                                   rsvd4[2];                     
    union   SDCOMP1LOCK_REG                  SDCOMP1LOCK;                  
    union   SDCOMP2CTL_REG                   SDCOMP2CTL;                   
    union   SDCOMP2EVT2FLTCTL_REG            SDCOMP2EVT2FLTCTL;            
    union   SDCOMP2EVT2FLTCLKCTL_REG         SDCOMP2EVT2FLTCLKCTL;         
    union   SDCOMP2EVT1FLTCTL_REG            SDCOMP2EVT1FLTCTL;            
    union   SDCOMP2EVT1FLTCLKCTL_REG         SDCOMP2EVT1FLTCLKCTL;         
    Uint16                                   rsvd5[2];                     
    union   SDCOMP2LOCK_REG                  SDCOMP2LOCK;                  
    union   SDCOMP3CTL_REG                   SDCOMP3CTL;                   
    union   SDCOMP3EVT2FLTCTL_REG            SDCOMP3EVT2FLTCTL;            
    union   SDCOMP3EVT2FLTCLKCTL_REG         SDCOMP3EVT2FLTCLKCTL;         
    union   SDCOMP3EVT1FLTCTL_REG            SDCOMP3EVT1FLTCTL;            
    union   SDCOMP3EVT1FLTCLKCTL_REG         SDCOMP3EVT1FLTCLKCTL;         
    Uint16                                   rsvd6[2];                     
    union   SDCOMP3LOCK_REG                  SDCOMP3LOCK;                  
    union   SDCOMP4CTL_REG                   SDCOMP4CTL;                   
    union   SDCOMP4EVT2FLTCTL_REG            SDCOMP4EVT2FLTCTL;            
    union   SDCOMP4EVT2FLTCLKCTL_REG         SDCOMP4EVT2FLTCLKCTL;         
    union   SDCOMP4EVT1FLTCTL_REG            SDCOMP4EVT1FLTCTL;            
    union   SDCOMP4EVT1FLTCLKCTL_REG         SDCOMP4EVT1FLTCLKCTL;         
    Uint16                                   rsvd7[2];                     
    union   SDCOMP4LOCK_REG                  SDCOMP4LOCK;                  
};




extern volatile struct SDFM_REGS Sdfm1Regs;
extern volatile struct SDFM_REGS Sdfm2Regs;



















































struct SPICCR_BITS {                    
    Uint16 SPICHAR:4;                   
    Uint16 SPILBK:1;                    
    Uint16 HS_MODE:1;                   
    Uint16 CLKPOLARITY:1;               
    Uint16 SPISWRESET:1;                
    Uint16 rsvd1:8;                     
};

union SPICCR_REG {
    Uint16  all;
    struct  SPICCR_BITS  bit;
};

struct SPICTL_BITS {                    
    Uint16 SPIINTENA:1;                 
    Uint16 TALK:1;                      
    Uint16 MASTER_SLAVE:1;              
    Uint16 CLK_PHASE:1;                 
    Uint16 OVERRUNINTENA:1;             
    Uint16 rsvd1:11;                    
};

union SPICTL_REG {
    Uint16  all;
    struct  SPICTL_BITS  bit;
};

struct SPISTS_BITS {                    
    Uint16 rsvd1:5;                     
    Uint16 BUFFULL_FLAG:1;              
    Uint16 INT_FLAG:1;                  
    Uint16 OVERRUN_FLAG:1;              
    Uint16 rsvd2:8;                     
};

union SPISTS_REG {
    Uint16  all;
    struct  SPISTS_BITS  bit;
};

struct SPIBRR_BITS {                    
    Uint16 SPI_BIT_RATE:7;              
    Uint16 rsvd1:9;                     
};

union SPIBRR_REG {
    Uint16  all;
    struct  SPIBRR_BITS  bit;
};

struct SPIFFTX_BITS {                   
    Uint16 TXFFIL:5;                    
    Uint16 TXFFIENA:1;                  
    Uint16 TXFFINTCLR:1;                
    Uint16 TXFFINT:1;                   
    Uint16 TXFFST:5;                    
    Uint16 TXFIFO:1;                    
    Uint16 SPIFFENA:1;                  
    Uint16 SPIRST:1;                    
};

union SPIFFTX_REG {
    Uint16  all;
    struct  SPIFFTX_BITS  bit;
};

struct SPIFFRX_BITS {                   
    Uint16 RXFFIL:5;                    
    Uint16 RXFFIENA:1;                  
    Uint16 RXFFINTCLR:1;                
    Uint16 RXFFINT:1;                   
    Uint16 RXFFST:5;                    
    Uint16 RXFIFORESET:1;               
    Uint16 RXFFOVFCLR:1;                
    Uint16 RXFFOVF:1;                   
};

union SPIFFRX_REG {
    Uint16  all;
    struct  SPIFFRX_BITS  bit;
};

struct SPIFFCT_BITS {                   
    Uint16 TXDLY:8;                     
    Uint16 rsvd1:8;                     
};

union SPIFFCT_REG {
    Uint16  all;
    struct  SPIFFCT_BITS  bit;
};

struct SPIPRI_BITS {                    
    Uint16 TRIWIRE:1;                   
    Uint16 STEINV:1;                    
    Uint16 rsvd1:2;                     
    Uint16 FREE:1;                      
    Uint16 SOFT:1;                      
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:9;                     
};

union SPIPRI_REG {
    Uint16  all;
    struct  SPIPRI_BITS  bit;
};

struct SPI_REGS {
    union   SPICCR_REG                       SPICCR;                       
    union   SPICTL_REG                       SPICTL;                       
    union   SPISTS_REG                       SPISTS;                       
    Uint16                                   rsvd1;                        
    union   SPIBRR_REG                       SPIBRR;                       
    Uint16                                   rsvd2;                        
    Uint16                                   SPIRXEMU;                     
    Uint16                                   SPIRXBUF;                     
    Uint16                                   SPITXBUF;                     
    Uint16                                   SPIDAT;                       
    union   SPIFFTX_REG                      SPIFFTX;                      
    union   SPIFFRX_REG                      SPIFFRX;                      
    union   SPIFFCT_REG                      SPIFFCT;                      
    Uint16                                   rsvd3[2];                     
    union   SPIPRI_REG                       SPIPRI;                       
};




extern volatile struct SPI_REGS SpiaRegs;
extern volatile struct SPI_REGS SpibRegs;



















































struct PARTIDL_BITS {                   
    Uint16 rsvd1:3;                     
    Uint16 rsvd2:2;                     
    Uint16 rsvd3:1;                     
    Uint16 QUAL:2;                      
    Uint16 PIN_COUNT:3;                 
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 INSTASPIN:2;                 
    Uint16 rsvd6:1;                     
    Uint16 FLASH_SIZE:8;                
    Uint16 rsvd7:4;                     
    Uint16 rsvd8:4;                     
};

union PARTIDL_REG {
    Uint32  all;
    struct  PARTIDL_BITS  bit;
};

struct PARTIDH_BITS {                   
    Uint16 rsvd1:4;                     
    Uint16 rsvd2:4;                     
    Uint16 FAMILY:8;                    
    Uint16 PARTNO:8;                    
    Uint16 DEVICE_CLASS_ID:8;           
};

union PARTIDH_REG {
    Uint32  all;
    struct  PARTIDH_BITS  bit;
};

struct REVID_BITS {                     
    Uint16 REVID:16;                    
    Uint16 rsvd1:16;                    
};

union REVID_REG {
    Uint32  all;
    struct  REVID_BITS  bit;
};

struct FUSEERR_BITS {                   
    Uint16 ALERR:5;                     
    Uint16 ERR:1;                       
    Uint16 rsvd1:10;                    
    Uint16 rsvd2:16;                    
};

union FUSEERR_REG {
    Uint32  all;
    struct  FUSEERR_BITS  bit;
};

struct SOFTPRES0_BITS {                 
    Uint16 CPU1_CLA1:1;                 
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:9;                     
    Uint16 CPU1_CPUBGCRC:1;             
    Uint16 CPU1_CLA1BGCRC:1;            
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:8;                     
    Uint16 CPU1_ERAD:1;                 
    Uint16 rsvd7:7;                     
};

union SOFTPRES0_REG {
    Uint32  all;
    struct  SOFTPRES0_BITS  bit;
};

struct SOFTPRES2_BITS {                 
    Uint16 EPWM1:1;                     
    Uint16 EPWM2:1;                     
    Uint16 EPWM3:1;                     
    Uint16 EPWM4:1;                     
    Uint16 EPWM5:1;                     
    Uint16 EPWM6:1;                     
    Uint16 EPWM7:1;                     
    Uint16 EPWM8:1;                     
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:1;                     
    Uint16 rsvd8:1;                     
    Uint16 rsvd9:16;                    
};

union SOFTPRES2_REG {
    Uint32  all;
    struct  SOFTPRES2_BITS  bit;
};

struct SOFTPRES3_BITS {                 
    Uint16 ECAP1:1;                     
    Uint16 ECAP2:1;                     
    Uint16 ECAP3:1;                     
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:8;                     
    Uint16 rsvd7:16;                    
};

union SOFTPRES3_REG {
    Uint32  all;
    struct  SOFTPRES3_BITS  bit;
};

struct SOFTPRES4_BITS {                 
    Uint16 EQEP1:1;                     
    Uint16 EQEP2:1;                     
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:12;                    
    Uint16 rsvd4:16;                    
};

union SOFTPRES4_REG {
    Uint32  all;
    struct  SOFTPRES4_BITS  bit;
};

struct SOFTPRES6_BITS {                 
    Uint16 SD1:1;                       
    Uint16 SD2:1;                       
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:8;                     
    Uint16 rsvd8:16;                    
};

union SOFTPRES6_REG {
    Uint32  all;
    struct  SOFTPRES6_BITS  bit;
};

struct SOFTPRES7_BITS {                 
    Uint16 SCI_A:1;                     
    Uint16 SCI_B:1;                     
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:12;                    
    Uint16 rsvd4:16;                    
};

union SOFTPRES7_REG {
    Uint32  all;
    struct  SOFTPRES7_BITS  bit;
};

struct SOFTPRES8_BITS {                 
    Uint16 SPI_A:1;                     
    Uint16 SPI_B:1;                     
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:12;                    
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:14;                    
};

union SOFTPRES8_REG {
    Uint32  all;
    struct  SOFTPRES8_BITS  bit;
};

struct SOFTPRES9_BITS {                 
    Uint16 I2C_A:1;                     
    Uint16 I2C_B:1;                     
    Uint16 rsvd1:14;                    
    Uint16 rsvd2:16;                    
};

union SOFTPRES9_REG {
    Uint32  all;
    struct  SOFTPRES9_BITS  bit;
};

struct SOFTPRES10_BITS {                
    Uint16 CAN_A:1;                     
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 MCAN_A:1;                    
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:8;                     
    Uint16 rsvd8:16;                    
};

union SOFTPRES10_REG {
    Uint32  all;
    struct  SOFTPRES10_BITS  bit;
};

struct SOFTPRES13_BITS {                
    Uint16 ADC_A:1;                     
    Uint16 ADC_B:1;                     
    Uint16 ADC_C:1;                     
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:12;                    
    Uint16 rsvd3:16;                    
};

union SOFTPRES13_REG {
    Uint32  all;
    struct  SOFTPRES13_BITS  bit;
};

struct SOFTPRES14_BITS {                
    Uint16 CMPSS1:1;                    
    Uint16 CMPSS2:1;                    
    Uint16 CMPSS3:1;                    
    Uint16 CMPSS4:1;                    
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:8;                     
    Uint16 rsvd6:16;                    
};

union SOFTPRES14_REG {
    Uint32  all;
    struct  SOFTPRES14_BITS  bit;
};

struct SOFTPRES16_BITS {                
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:12;                    
    Uint16 DAC_A:1;                     
    Uint16 DAC_B:1;                     
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:1;                     
    Uint16 rsvd8:12;                    
};

union SOFTPRES16_REG {
    Uint32  all;
    struct  SOFTPRES16_BITS  bit;
};

struct SOFTPRES17_BITS {                
    Uint16 CLB1:1;                      
    Uint16 CLB2:1;                      
    Uint16 CLB3:1;                      
    Uint16 CLB4:1;                      
    Uint16 rsvd1:12;                    
    Uint16 rsvd2:16;                    
};

union SOFTPRES17_REG {
    Uint32  all;
    struct  SOFTPRES17_BITS  bit;
};

struct SOFTPRES18_BITS {                
    Uint16 FSITX_A:1;                   
    Uint16 FSIRX_A:1;                   
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:12;                    
    Uint16 rsvd4:16;                    
};

union SOFTPRES18_REG {
    Uint32  all;
    struct  SOFTPRES18_BITS  bit;
};

struct SOFTPRES19_BITS {                
    Uint16 LIN_A:1;                     
    Uint16 LIN_B:1;                     
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:12;                    
    Uint16 rsvd4:16;                    
};

union SOFTPRES19_REG {
    Uint32  all;
    struct  SOFTPRES19_BITS  bit;
};

struct SOFTPRES20_BITS {                
    Uint16 PMBUS_A:1;                   
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:14;                    
    Uint16 rsvd3:16;                    
};

union SOFTPRES20_REG {
    Uint32  all;
    struct  SOFTPRES20_BITS  bit;
};

struct SOFTPRES21_BITS {                
    Uint16 DCC0:1;                      
    Uint16 DCC1:1;                      
    Uint16 rsvd1:14;                    
    Uint16 rsvd2:16;                    
};

union SOFTPRES21_REG {
    Uint32  all;
    struct  SOFTPRES21_BITS  bit;
};

struct SOFTPRES25_BITS {                
    Uint16 HIC_A:1;                     
    Uint16 rsvd1:15;                    
    Uint16 rsvd2:16;                    
};

union SOFTPRES25_REG {
    Uint32  all;
    struct  SOFTPRES25_BITS  bit;
};

struct SOFTPRES26_BITS {                
    Uint16 AESA:1;                      
    Uint16 rsvd1:15;                    
    Uint16 rsvd2:16;                    
};

union SOFTPRES26_REG {
    Uint32  all;
    struct  SOFTPRES26_BITS  bit;
};

struct SOFTPRES27_BITS {                
    Uint16 EPG1:1;                      
    Uint16 rsvd1:15;                    
    Uint16 rsvd2:16;                    
};

union SOFTPRES27_REG {
    Uint32  all;
    struct  SOFTPRES27_BITS  bit;
};

struct TAP_STATUS_BITS {                
    Uint16 TAP_STATE:16;                
    Uint16 rsvd1:15;                    
    Uint16 DCON:1;                      
};

union TAP_STATUS_REG {
    Uint32  all;
    struct  TAP_STATUS_BITS  bit;
};

struct ECAPTYPE_BITS {                  
    Uint16 TYPE:2;                      
    Uint16 rsvd1:13;                    
    Uint16 LOCK:1;                      
};

union ECAPTYPE_REG {
    Uint16  all;
    struct  ECAPTYPE_BITS  bit;
};

struct SDFMTYPE_BITS {                  
    Uint16 TYPE:2;                      
    Uint16 rsvd1:13;                    
    Uint16 LOCK:1;                      
};

union SDFMTYPE_REG {
    Uint16  all;
    struct  SDFMTYPE_BITS  bit;
};

struct DEV_CFG_REGS {
    Uint16                                   rsvd1[8];                     
    union   PARTIDL_REG                      PARTIDL;                      
    union   PARTIDH_REG                      PARTIDH;                      
    union   REVID_REG                        REVID;                        
    Uint16                                   rsvd2[102];                   
    union   FUSEERR_REG                      FUSEERR;                      
    Uint16                                   rsvd3[12];                    
    union   SOFTPRES0_REG                    SOFTPRES0;                    
    Uint16                                   rsvd4[2];                     
    union   SOFTPRES2_REG                    SOFTPRES2;                    
    union   SOFTPRES3_REG                    SOFTPRES3;                    
    union   SOFTPRES4_REG                    SOFTPRES4;                    
    Uint16                                   rsvd5[2];                     
    union   SOFTPRES6_REG                    SOFTPRES6;                    
    union   SOFTPRES7_REG                    SOFTPRES7;                    
    union   SOFTPRES8_REG                    SOFTPRES8;                    
    union   SOFTPRES9_REG                    SOFTPRES9;                    
    union   SOFTPRES10_REG                   SOFTPRES10;                   
    Uint16                                   rsvd6[4];                     
    union   SOFTPRES13_REG                   SOFTPRES13;                   
    union   SOFTPRES14_REG                   SOFTPRES14;                   
    Uint16                                   rsvd7[2];                     
    union   SOFTPRES16_REG                   SOFTPRES16;                   
    union   SOFTPRES17_REG                   SOFTPRES17;                   
    union   SOFTPRES18_REG                   SOFTPRES18;                   
    union   SOFTPRES19_REG                   SOFTPRES19;                   
    union   SOFTPRES20_REG                   SOFTPRES20;                   
    union   SOFTPRES21_REG                   SOFTPRES21;                   
    Uint16                                   rsvd8[6];                     
    union   SOFTPRES25_REG                   SOFTPRES25;                   
    union   SOFTPRES26_REG                   SOFTPRES26;                   
    union   SOFTPRES27_REG                   SOFTPRES27;                   
    Uint16                                   rsvd9[118];                   
    union   TAP_STATUS_REG                   TAP_STATUS;                   
    Uint16                                   rsvd10[105];                  
    union   ECAPTYPE_REG                     ECAPTYPE;                     
    union   SDFMTYPE_REG                     SDFMTYPE;                     
};

struct CLKCFGLOCK1_BITS {               
    Uint16 CLKSRCCTL1:1;                
    Uint16 CLKSRCCTL2:1;                
    Uint16 CLKSRCCTL3:1;                
    Uint16 SYSPLLCTL1:1;                
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 SYSPLLMULT:1;                
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:1;                     
    Uint16 SYSCLKDIVSEL:1;              
    Uint16 AUXCLKDIVSEL:1;              
    Uint16 rsvd7:1;                     
    Uint16 rsvd8:1;                     
    Uint16 LOSPCP:1;                    
    Uint16 XTALCR:1;                    
    Uint16 rsvd9:15;                    
};

union CLKCFGLOCK1_REG {
    Uint32  all;
    struct  CLKCFGLOCK1_BITS  bit;
};

struct CLKSRCCTL1_BITS {                
    Uint16 OSCCLKSRCSEL:2;              
    Uint16 rsvd1:1;                     
    Uint16 INTOSC2OFF:1;                
    Uint16 rsvd2:1;                     
    Uint16 WDHALTI:1;                   
    Uint16 rsvd3:10;                    
    Uint16 rsvd4:16;                    
};

union CLKSRCCTL1_REG {
    Uint32  all;
    struct  CLKSRCCTL1_BITS  bit;
};

struct CLKSRCCTL2_BITS {                
    Uint16 rsvd1:2;                     
    Uint16 CANABCLKSEL:2;               
    Uint16 rsvd2:2;                     
    Uint16 rsvd3:2;                     
    Uint16 rsvd4:2;                     
    Uint16 MCANABCLKSEL:2;              
    Uint16 rsvd5:2;                     
    Uint16 rsvd6:2;                     
    Uint16 rsvd7:2;                     
    Uint16 rsvd8:14;                    
};

union CLKSRCCTL2_REG {
    Uint32  all;
    struct  CLKSRCCTL2_BITS  bit;
};

struct CLKSRCCTL3_BITS {                
    Uint16 XCLKOUTSEL:4;                
    Uint16 rsvd1:12;                    
    Uint16 rsvd2:16;                    
};

union CLKSRCCTL3_REG {
    Uint32  all;
    struct  CLKSRCCTL3_BITS  bit;
};

struct SYSPLLCTL1_BITS {                
    Uint16 PLLEN:1;                     
    Uint16 PLLCLKEN:1;                  
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:10;                    
    Uint16 rsvd6:16;                    
};

union SYSPLLCTL1_REG {
    Uint32  all;
    struct  SYSPLLCTL1_BITS  bit;
};

struct SYSPLLMULT_BITS {                
    Uint16 IMULT:8;                     
    Uint16 rsvd1:2;                     
    Uint16 rsvd2:2;                     
    Uint16 rsvd3:2;                     
    Uint16 rsvd4:2;                     
    Uint16 ODIV:5;                      
    Uint16 rsvd5:3;                     
    Uint16 REFDIV:5;                    
    Uint16 rsvd6:3;                     
};

union SYSPLLMULT_REG {
    Uint32  all;
    struct  SYSPLLMULT_BITS  bit;
};

struct SYSPLLSTS_BITS {                 
    Uint16 LOCKS:1;                     
    Uint16 SLIPS:1;                     
    Uint16 rsvd1:1;                     
    Uint16 REF_LOSTS:1;                 
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:10;                    
    Uint16 rsvd5:16;                    
};

union SYSPLLSTS_REG {
    Uint32  all;
    struct  SYSPLLSTS_BITS  bit;
};

struct SYSCLKDIVSEL_BITS {              
    Uint16 PLLSYSCLKDIV:6;              
    Uint16 rsvd1:2;                     
    Uint16 PLLSYSCLKDIV_LSB:1;          
    Uint16 rsvd2:7;                     
    Uint16 rsvd3:16;                    
};

union SYSCLKDIVSEL_REG {
    Uint32  all;
    struct  SYSCLKDIVSEL_BITS  bit;
};

struct AUXCLKDIVSEL_BITS {              
    Uint16 rsvd1:3;                     
    Uint16 rsvd2:5;                     
    Uint16 MCANCLKDIV:5;                
    Uint16 rsvd3:3;                     
    Uint16 rsvd4:16;                    
};

union AUXCLKDIVSEL_REG {
    Uint32  all;
    struct  AUXCLKDIVSEL_BITS  bit;
};

struct XCLKOUTDIVSEL_BITS {             
    Uint16 XCLKOUTDIV:2;                
    Uint16 rsvd1:14;                    
    Uint16 rsvd2:16;                    
};

union XCLKOUTDIVSEL_REG {
    Uint32  all;
    struct  XCLKOUTDIVSEL_BITS  bit;
};

struct LOSPCP_BITS {                    
    Uint16 LSPCLKDIV:3;                 
    Uint16 rsvd1:13;                    
    Uint16 rsvd2:16;                    
};

union LOSPCP_REG {
    Uint32  all;
    struct  LOSPCP_BITS  bit;
};

struct MCDCR_BITS {                     
    Uint16 MCLKSTS:1;                   
    Uint16 MCLKCLR:1;                   
    Uint16 MCLKOFF:1;                   
    Uint16 OSCOFF:1;                    
    Uint16 SYSREF_LOSTS:1;              
    Uint16 SYSREF_LOSTSCLR:1;           
    Uint16 SYSREF_LOST_MCD_EN:1;        
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:6;                     
    Uint16 rsvd5:16;                    
};

union MCDCR_REG {
    Uint32  all;
    struct  MCDCR_BITS  bit;
};

struct X1CNT_BITS {                     
    Uint16 X1CNT:11;                    
    Uint16 rsvd1:5;                     
    Uint16 CLR:1;                       
    Uint16 rsvd2:15;                    
};

union X1CNT_REG {
    Uint32  all;
    struct  X1CNT_BITS  bit;
};

struct XTALCR_BITS {                    
    Uint16 OSCOFF:1;                    
    Uint16 SE:1;                        
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:13;                    
    Uint16 rsvd3:16;                    
};

union XTALCR_REG {
    Uint32  all;
    struct  XTALCR_BITS  bit;
};

struct XTALCR2_BITS {                   
    Uint16 XIF:1;                       
    Uint16 XOF:1;                       
    Uint16 FEN:1;                       
    Uint16 rsvd1:13;                    
    Uint16 rsvd2:16;                    
};

union XTALCR2_REG {
    Uint32  all;
    struct  XTALCR2_BITS  bit;
};

struct CLKFAILCFG_BITS {                
    Uint16 DCC0_ERROR_EN:1;             
    Uint16 DCC1_ERROR_EN:1;             
    Uint16 rsvd1:14;                    
    Uint16 rsvd2:16;                    
};

union CLKFAILCFG_REG {
    Uint32  all;
    struct  CLKFAILCFG_BITS  bit;
};

struct CLK_CFG_REGS {
    Uint16                                   rsvd1[2];                     
    union   CLKCFGLOCK1_REG                  CLKCFGLOCK1;                  
    Uint16                                   rsvd2[4];                     
    union   CLKSRCCTL1_REG                   CLKSRCCTL1;                   
    union   CLKSRCCTL2_REG                   CLKSRCCTL2;                   
    union   CLKSRCCTL3_REG                   CLKSRCCTL3;                   
    union   SYSPLLCTL1_REG                   SYSPLLCTL1;                   
    Uint16                                   rsvd3[4];                     
    union   SYSPLLMULT_REG                   SYSPLLMULT;                   
    union   SYSPLLSTS_REG                    SYSPLLSTS;                    
    Uint16                                   rsvd4[10];                    
    union   SYSCLKDIVSEL_REG                 SYSCLKDIVSEL;                 
    union   AUXCLKDIVSEL_REG                 AUXCLKDIVSEL;                 
    Uint16                                   rsvd5[2];                     
    union   XCLKOUTDIVSEL_REG                XCLKOUTDIVSEL;                
    Uint16                                   rsvd6[2];                     
    union   LOSPCP_REG                       LOSPCP;                       
    union   MCDCR_REG                        MCDCR;                        
    union   X1CNT_REG                        X1CNT;                        
    union   XTALCR_REG                       XTALCR;                       
    Uint16                                   rsvd7[6];                     
    union   XTALCR2_REG                      XTALCR2;                      
    union   CLKFAILCFG_REG                   CLKFAILCFG;                   
};

struct CPUSYSLOCK1_BITS {               
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 PIEVERRADDR:1;               
    Uint16 PCLKCR0:1;                   
    Uint16 rsvd3:1;                     
    Uint16 PCLKCR2:1;                   
    Uint16 PCLKCR3:1;                   
    Uint16 PCLKCR4:1;                   
    Uint16 rsvd4:1;                     
    Uint16 PCLKCR6:1;                   
    Uint16 PCLKCR7:1;                   
    Uint16 PCLKCR8:1;                   
    Uint16 PCLKCR9:1;                   
    Uint16 PCLKCR10:1;                  
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:1;                     
    Uint16 PCLKCR13:1;                  
    Uint16 PCLKCR14:1;                  
    Uint16 rsvd7:1;                     
    Uint16 PCLKCR16:1;                  
    Uint16 rsvd8:1;                     
    Uint16 LPMCR:1;                     
    Uint16 GPIOLPMSEL0:1;               
    Uint16 GPIOLPMSEL1:1;               
    Uint16 PCLKCR17:1;                  
    Uint16 PCLKCR18:1;                  
    Uint16 PCLKCR19:1;                  
    Uint16 PCLKCR20:1;                  
    Uint16 PCLKCR21:1;                  
    Uint16 PCLKCR22:1;                  
    Uint16 rsvd9:1;                     
    Uint16 rsvd10:1;                    
};

union CPUSYSLOCK1_REG {
    Uint32  all;
    struct  CPUSYSLOCK1_BITS  bit;
};

struct CPUSYSLOCK2_BITS {               
    Uint16 rsvd1:1;                     
    Uint16 PCLKCR25:1;                  
    Uint16 PCLKCR26:1;                  
    Uint16 PCLKCR27:1;                  
    Uint16 rsvd2:12;                    
    Uint16 rsvd3:16;                    
};

union CPUSYSLOCK2_REG {
    Uint32  all;
    struct  CPUSYSLOCK2_BITS  bit;
};

struct PIEVERRADDR_BITS {               
    Uint32 ADDR:22;                     
    Uint16 rsvd1:10;                    
};

union PIEVERRADDR_REG {
    Uint32  all;
    struct  PIEVERRADDR_BITS  bit;
};

struct PCLKCR0_BITS {                   
    Uint16 CLA1:1;                      
    Uint16 rsvd1:1;                     
    Uint16 DMA:1;                       
    Uint16 CPUTIMER0:1;                 
    Uint16 CPUTIMER1:1;                 
    Uint16 CPUTIMER2:1;                 
    Uint16 rsvd2:7;                     
    Uint16 CPUBGCRC:1;                  
    Uint16 CLA1BGCRC:1;                 
    Uint16 rsvd3:1;                     
    Uint16 HRCAL:1;                     
    Uint16 rsvd4:1;                     
    Uint16 TBCLKSYNC:1;                 
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:4;                     
    Uint16 ERAD:1;                      
    Uint16 rsvd7:7;                     
};

union PCLKCR0_REG {
    Uint32  all;
    struct  PCLKCR0_BITS  bit;
};

struct PCLKCR2_BITS {                   
    Uint16 EPWM1:1;                     
    Uint16 EPWM2:1;                     
    Uint16 EPWM3:1;                     
    Uint16 EPWM4:1;                     
    Uint16 EPWM5:1;                     
    Uint16 EPWM6:1;                     
    Uint16 EPWM7:1;                     
    Uint16 EPWM8:1;                     
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:1;                     
    Uint16 rsvd8:1;                     
    Uint16 rsvd9:16;                    
};

union PCLKCR2_REG {
    Uint32  all;
    struct  PCLKCR2_BITS  bit;
};

struct PCLKCR3_BITS {                   
    Uint16 ECAP1:1;                     
    Uint16 ECAP2:1;                     
    Uint16 ECAP3:1;                     
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:8;                     
    Uint16 rsvd7:16;                    
};

union PCLKCR3_REG {
    Uint32  all;
    struct  PCLKCR3_BITS  bit;
};

struct PCLKCR4_BITS {                   
    Uint16 EQEP1:1;                     
    Uint16 EQEP2:1;                     
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:12;                    
    Uint16 rsvd4:16;                    
};

union PCLKCR4_REG {
    Uint32  all;
    struct  PCLKCR4_BITS  bit;
};

struct PCLKCR6_BITS {                   
    Uint16 SD1:1;                       
    Uint16 SD2:1;                       
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:8;                     
    Uint16 rsvd8:16;                    
};

union PCLKCR6_REG {
    Uint32  all;
    struct  PCLKCR6_BITS  bit;
};

struct PCLKCR7_BITS {                   
    Uint16 SCI_A:1;                     
    Uint16 SCI_B:1;                     
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:12;                    
    Uint16 rsvd4:16;                    
};

union PCLKCR7_REG {
    Uint32  all;
    struct  PCLKCR7_BITS  bit;
};

struct PCLKCR8_BITS {                   
    Uint16 SPI_A:1;                     
    Uint16 SPI_B:1;                     
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:12;                    
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:14;                    
};

union PCLKCR8_REG {
    Uint32  all;
    struct  PCLKCR8_BITS  bit;
};

struct PCLKCR9_BITS {                   
    Uint16 I2C_A:1;                     
    Uint16 I2C_B:1;                     
    Uint16 rsvd1:14;                    
    Uint16 rsvd2:16;                    
};

union PCLKCR9_REG {
    Uint32  all;
    struct  PCLKCR9_BITS  bit;
};

struct PCLKCR10_BITS {                  
    Uint16 CAN_A:1;                     
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 MCAN_A:1;                    
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:8;                     
    Uint16 rsvd8:16;                    
};

union PCLKCR10_REG {
    Uint32  all;
    struct  PCLKCR10_BITS  bit;
};

struct PCLKCR13_BITS {                  
    Uint16 ADC_A:1;                     
    Uint16 ADC_B:1;                     
    Uint16 ADC_C:1;                     
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:12;                    
    Uint16 rsvd3:16;                    
};

union PCLKCR13_REG {
    Uint32  all;
    struct  PCLKCR13_BITS  bit;
};

struct PCLKCR14_BITS {                  
    Uint16 CMPSS1:1;                    
    Uint16 CMPSS2:1;                    
    Uint16 CMPSS3:1;                    
    Uint16 CMPSS4:1;                    
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:8;                     
    Uint16 rsvd6:16;                    
};

union PCLKCR14_REG {
    Uint32  all;
    struct  PCLKCR14_BITS  bit;
};

struct PCLKCR16_BITS {                  
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:12;                    
    Uint16 DAC_A:1;                     
    Uint16 DAC_B:1;                     
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:1;                     
    Uint16 rsvd8:12;                    
};

union PCLKCR16_REG {
    Uint32  all;
    struct  PCLKCR16_BITS  bit;
};

struct PCLKCR17_BITS {                  
    Uint16 CLB1:1;                      
    Uint16 CLB2:1;                      
    Uint16 CLB3:1;                      
    Uint16 CLB4:1;                      
    Uint16 rsvd1:12;                    
    Uint16 rsvd2:16;                    
};

union PCLKCR17_REG {
    Uint32  all;
    struct  PCLKCR17_BITS  bit;
};

struct PCLKCR18_BITS {                  
    Uint16 FSITX_A:1;                   
    Uint16 FSIRX_A:1;                   
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:12;                    
    Uint16 rsvd4:16;                    
};

union PCLKCR18_REG {
    Uint32  all;
    struct  PCLKCR18_BITS  bit;
};

struct PCLKCR19_BITS {                  
    Uint16 LIN_A:1;                     
    Uint16 LIN_B:1;                     
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:12;                    
    Uint16 rsvd4:16;                    
};

union PCLKCR19_REG {
    Uint32  all;
    struct  PCLKCR19_BITS  bit;
};

struct PCLKCR20_BITS {                  
    Uint16 PMBUS_A:1;                   
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:14;                    
    Uint16 rsvd3:16;                    
};

union PCLKCR20_REG {
    Uint32  all;
    struct  PCLKCR20_BITS  bit;
};

struct PCLKCR21_BITS {                  
    Uint16 DCC0:1;                      
    Uint16 DCC1:1;                      
    Uint16 rsvd1:14;                    
    Uint16 rsvd2:16;                    
};

union PCLKCR21_REG {
    Uint32  all;
    struct  PCLKCR21_BITS  bit;
};

struct PCLKCR25_BITS {                  
    Uint16 HICA:1;                      
    Uint16 rsvd1:15;                    
    Uint16 rsvd2:16;                    
};

union PCLKCR25_REG {
    Uint32  all;
    struct  PCLKCR25_BITS  bit;
};

struct PCLKCR26_BITS {                  
    Uint16 AESA:1;                      
    Uint16 rsvd1:15;                    
    Uint16 rsvd2:16;                    
};

union PCLKCR26_REG {
    Uint32  all;
    struct  PCLKCR26_BITS  bit;
};

struct PCLKCR27_BITS {                  
    Uint16 EPG1:1;                      
    Uint16 rsvd1:15;                    
    Uint16 rsvd2:16;                    
};

union PCLKCR27_REG {
    Uint32  all;
    struct  PCLKCR27_BITS  bit;
};

struct SIMRESET_BITS {                  
    Uint16 CPU1RSn:1;                   
    Uint16 XRSn:1;                      
    Uint16 rsvd1:14;                    
    Uint16 KEY:16;                      
};

union SIMRESET_REG {
    Uint32  all;
    struct  SIMRESET_BITS  bit;
};

struct LPMCR_BITS {                     
    Uint16 LPM:2;                       
    Uint16 QUALSTDBY:6;                 
    Uint16 rsvd1:7;                     
    Uint16 WDINTE:1;                    
    Uint16 rsvd2:2;                     
    Uint16 rsvd3:13;                    
    Uint16 rsvd4:1;                     
};

union LPMCR_REG {
    Uint32  all;
    struct  LPMCR_BITS  bit;
};

struct GPIOLPMSEL0_BITS {               
    Uint16 GPIO0:1;                     
    Uint16 GPIO1:1;                     
    Uint16 GPIO2:1;                     
    Uint16 GPIO3:1;                     
    Uint16 GPIO4:1;                     
    Uint16 GPIO5:1;                     
    Uint16 GPIO6:1;                     
    Uint16 GPIO7:1;                     
    Uint16 GPIO8:1;                     
    Uint16 GPIO9:1;                     
    Uint16 GPIO10:1;                    
    Uint16 GPIO11:1;                    
    Uint16 GPIO12:1;                    
    Uint16 GPIO13:1;                    
    Uint16 GPIO14:1;                    
    Uint16 GPIO15:1;                    
    Uint16 GPIO16:1;                    
    Uint16 GPIO17:1;                    
    Uint16 GPIO18:1;                    
    Uint16 GPIO19:1;                    
    Uint16 GPIO20:1;                    
    Uint16 GPIO21:1;                    
    Uint16 GPIO22:1;                    
    Uint16 GPIO23:1;                    
    Uint16 GPIO24:1;                    
    Uint16 GPIO25:1;                    
    Uint16 GPIO26:1;                    
    Uint16 GPIO27:1;                    
    Uint16 GPIO28:1;                    
    Uint16 GPIO29:1;                    
    Uint16 GPIO30:1;                    
    Uint16 GPIO31:1;                    
};

union GPIOLPMSEL0_REG {
    Uint32  all;
    struct  GPIOLPMSEL0_BITS  bit;
};

struct GPIOLPMSEL1_BITS {               
    Uint16 GPIO32:1;                    
    Uint16 GPIO33:1;                    
    Uint16 GPIO34:1;                    
    Uint16 GPIO35:1;                    
    Uint16 rsvd1:1;                     
    Uint16 GPIO37:1;                    
    Uint16 rsvd2:1;                     
    Uint16 GPIO39:1;                    
    Uint16 GPIO40:1;                    
    Uint16 GPIO41:1;                    
    Uint16 GPIO42:1;                    
    Uint16 GPIO43:1;                    
    Uint16 GPIO44:1;                    
    Uint16 GPIO45:1;                    
    Uint16 GPIO46:1;                    
    Uint16 GPIO47:1;                    
    Uint16 GPIO48:1;                    
    Uint16 GPIO49:1;                    
    Uint16 GPIO50:1;                    
    Uint16 GPIO51:1;                    
    Uint16 GPIO52:1;                    
    Uint16 GPIO53:1;                    
    Uint16 GPIO54:1;                    
    Uint16 GPIO55:1;                    
    Uint16 GPIO56:1;                    
    Uint16 GPIO57:1;                    
    Uint16 GPIO58:1;                    
    Uint16 GPIO59:1;                    
    Uint16 GPIO60:1;                    
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
};

union GPIOLPMSEL1_REG {
    Uint32  all;
    struct  GPIOLPMSEL1_BITS  bit;
};

struct TMR2CLKCTL_BITS {                
    Uint16 TMR2CLKSRCSEL:3;             
    Uint16 TMR2CLKPRESCALE:3;           
    Uint16 rsvd1:10;                    
    Uint16 rsvd2:16;                    
};

union TMR2CLKCTL_REG {
    Uint32  all;
    struct  TMR2CLKCTL_BITS  bit;
};

struct RESCCLR_BITS {                   
    Uint16 POR:1;                       
    Uint16 XRSn:1;                      
    Uint16 WDRSn:1;                     
    Uint16 NMIWDRSn:1;                  
    Uint16 rsvd1:1;                     
    Uint16 HWBISTn:1;                   
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 SCCRESETn:1;                 
    Uint16 rsvd4:1;                     
    Uint16 SIMRESET_CPU1RSn:1;          
    Uint16 SIMRESET_XRSn:1;             
    Uint16 rsvd5:4;                     
    Uint16 rsvd6:16;                    
};

union RESCCLR_REG {
    Uint32  all;
    struct  RESCCLR_BITS  bit;
};

struct RESC_BITS {                      
    Uint16 POR:1;                       
    Uint16 XRSn:1;                      
    Uint16 WDRSn:1;                     
    Uint16 NMIWDRSn:1;                  
    Uint16 rsvd1:1;                     
    Uint16 HWBISTn:1;                   
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 SCCRESETn:1;                 
    Uint16 rsvd4:1;                     
    Uint16 SIMRESET_CPU1RSn:1;          
    Uint16 SIMRESET_XRSn:1;             
    Uint16 rsvd5:4;                     
    Uint16 rsvd6:14;                    
    Uint16 XRSn_pin_status:1;           
    Uint16 DCON:1;                      
};

union RESC_REG {
    Uint32  all;
    struct  RESC_BITS  bit;
};

struct MCANWAKESTATUS_BITS {            
    Uint16 WAKE:1;                      
    Uint16 rsvd1:15;                    
    Uint16 rsvd2:16;                    
};

union MCANWAKESTATUS_REG {
    Uint32  all;
    struct  MCANWAKESTATUS_BITS  bit;
};

struct MCANWAKESTATUSCLR_BITS {         
    Uint16 WAKE:1;                      
    Uint16 rsvd1:15;                    
    Uint16 rsvd2:16;                    
};

union MCANWAKESTATUSCLR_REG {
    Uint32  all;
    struct  MCANWAKESTATUSCLR_BITS  bit;
};

struct CLKSTOPREQ_BITS {                
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:2;                     
    Uint16 MCAN_A:1;                    
    Uint16 rsvd8:3;                     
    Uint16 rsvd9:4;                     
    Uint16 KEY:16;                      
};

union CLKSTOPREQ_REG {
    Uint32  all;
    struct  CLKSTOPREQ_BITS  bit;
};

struct CLKSTOPACK_BITS {                
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:2;                     
    Uint16 MCAN_A:1;                    
    Uint16 rsvd8:7;                     
    Uint16 rsvd9:16;                    
};

union CLKSTOPACK_REG {
    Uint32  all;
    struct  CLKSTOPACK_BITS  bit;
};

struct CPU_SYS_REGS {
    union   CPUSYSLOCK1_REG                  CPUSYSLOCK1;                  
    union   CPUSYSLOCK2_REG                  CPUSYSLOCK2;                  
    Uint16                                   rsvd1[6];                     
    union   PIEVERRADDR_REG                  PIEVERRADDR;                  
    Uint16                                   rsvd2[22];                    
    union   PCLKCR0_REG                      PCLKCR0;                      
    Uint16                                   rsvd3[2];                     
    union   PCLKCR2_REG                      PCLKCR2;                      
    union   PCLKCR3_REG                      PCLKCR3;                      
    union   PCLKCR4_REG                      PCLKCR4;                      
    Uint16                                   rsvd4[2];                     
    union   PCLKCR6_REG                      PCLKCR6;                      
    union   PCLKCR7_REG                      PCLKCR7;                      
    union   PCLKCR8_REG                      PCLKCR8;                      
    union   PCLKCR9_REG                      PCLKCR9;                      
    union   PCLKCR10_REG                     PCLKCR10;                     
    Uint16                                   rsvd5[4];                     
    union   PCLKCR13_REG                     PCLKCR13;                     
    union   PCLKCR14_REG                     PCLKCR14;                     
    Uint16                                   rsvd6[2];                     
    union   PCLKCR16_REG                     PCLKCR16;                     
    union   PCLKCR17_REG                     PCLKCR17;                     
    union   PCLKCR18_REG                     PCLKCR18;                     
    union   PCLKCR19_REG                     PCLKCR19;                     
    union   PCLKCR20_REG                     PCLKCR20;                     
    union   PCLKCR21_REG                     PCLKCR21;                     
    Uint16                                   rsvd7[6];                     
    union   PCLKCR25_REG                     PCLKCR25;                     
    union   PCLKCR26_REG                     PCLKCR26;                     
    union   PCLKCR27_REG                     PCLKCR27;                     
    Uint16                                   rsvd8[22];                    
    union   SIMRESET_REG                     SIMRESET;                     
    Uint16                                   rsvd9[4];                     
    union   LPMCR_REG                        LPMCR;                        
    union   GPIOLPMSEL0_REG                  GPIOLPMSEL0;                  
    union   GPIOLPMSEL1_REG                  GPIOLPMSEL1;                  
    union   TMR2CLKCTL_REG                   TMR2CLKCTL;                   
    union   RESCCLR_REG                      RESCCLR;                      
    union   RESC_REG                         RESC;                         
    Uint16                                   rsvd10[22];                   
    union   MCANWAKESTATUS_REG               MCANWAKESTATUS;               
    union   MCANWAKESTATUSCLR_REG            MCANWAKESTATUSCLR;            
    union   CLKSTOPREQ_REG                   CLKSTOPREQ;                   
    union   CLKSTOPACK_REG                   CLKSTOPACK;                   
};

struct CLA1TASKSRCSELLOCK_BITS {        
    Uint16 CLA1TASKSRCSEL1:1;           
    Uint16 CLA1TASKSRCSEL2:1;           
    Uint16 rsvd1:14;                    
    Uint16 rsvd2:16;                    
};

union CLA1TASKSRCSELLOCK_REG {
    Uint32  all;
    struct  CLA1TASKSRCSELLOCK_BITS  bit;
};

struct DMACHSRCSELLOCK_BITS {           
    Uint16 DMACHSRCSEL1:1;              
    Uint16 DMACHSRCSEL2:1;              
    Uint16 rsvd1:14;                    
    Uint16 rsvd2:16;                    
};

union DMACHSRCSELLOCK_REG {
    Uint32  all;
    struct  DMACHSRCSELLOCK_BITS  bit;
};

struct CLA1TASKSRCSEL1_BITS {           
    Uint16 TASK1:8;                     
    Uint16 TASK2:8;                     
    Uint16 TASK3:8;                     
    Uint16 TASK4:8;                     
};

union CLA1TASKSRCSEL1_REG {
    Uint32  all;
    struct  CLA1TASKSRCSEL1_BITS  bit;
};

struct CLA1TASKSRCSEL2_BITS {           
    Uint16 TASK5:8;                     
    Uint16 TASK6:8;                     
    Uint16 TASK7:8;                     
    Uint16 TASK8:8;                     
};

union CLA1TASKSRCSEL2_REG {
    Uint32  all;
    struct  CLA1TASKSRCSEL2_BITS  bit;
};

struct DMACHSRCSEL1_BITS {              
    Uint16 CH1:8;                       
    Uint16 CH2:8;                       
    Uint16 CH3:8;                       
    Uint16 CH4:8;                       
};

union DMACHSRCSEL1_REG {
    Uint32  all;
    struct  DMACHSRCSEL1_BITS  bit;
};

struct DMACHSRCSEL2_BITS {              
    Uint16 CH5:8;                       
    Uint16 CH6:8;                       
    Uint16 rsvd1:16;                    
};

union DMACHSRCSEL2_REG {
    Uint32  all;
    struct  DMACHSRCSEL2_BITS  bit;
};

struct DMA_CLA_SRC_SEL_REGS {
    union   CLA1TASKSRCSELLOCK_REG           CLA1TASKSRCSELLOCK;           
    Uint16                                   rsvd1[2];                     
    union   DMACHSRCSELLOCK_REG              DMACHSRCSELLOCK;              
    union   CLA1TASKSRCSEL1_REG              CLA1TASKSRCSEL1;              
    union   CLA1TASKSRCSEL2_REG              CLA1TASKSRCSEL2;              
    Uint16                                   rsvd2[12];                    
    union   DMACHSRCSEL1_REG                 DMACHSRCSEL1;                 
    union   DMACHSRCSEL2_REG                 DMACHSRCSEL2;                 
};

struct ADCA_AC_BITS {                   
    Uint16 CPU1_ACC:2;                  
    Uint16 CLA1_ACC:2;                  
    Uint16 rsvd1:2;                     
    Uint16 rsvd2:2;                     
    Uint16 rsvd3:8;                     
    Uint16 rsvd4:16;                    
};

union ADCA_AC_REG {
    Uint32  all;
    struct  ADCA_AC_BITS  bit;
};

struct ADCB_AC_BITS {                   
    Uint16 CPU1_ACC:2;                  
    Uint16 CLA1_ACC:2;                  
    Uint16 rsvd1:2;                     
    Uint16 rsvd2:2;                     
    Uint16 rsvd3:8;                     
    Uint16 rsvd4:16;                    
};

union ADCB_AC_REG {
    Uint32  all;
    struct  ADCB_AC_BITS  bit;
};

struct ADCC_AC_BITS {                   
    Uint16 CPU1_ACC:2;                  
    Uint16 CLA1_ACC:2;                  
    Uint16 rsvd1:2;                     
    Uint16 rsvd2:2;                     
    Uint16 rsvd3:8;                     
    Uint16 rsvd4:16;                    
};

union ADCC_AC_REG {
    Uint32  all;
    struct  ADCC_AC_BITS  bit;
};

struct CMPSS1_AC_BITS {                 
    Uint16 CPU1_ACC:2;                  
    Uint16 CLA1_ACC:2;                  
    Uint16 DMA1_ACC:2;                  
    Uint16 HICA_ACC:2;                  
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union CMPSS1_AC_REG {
    Uint32  all;
    struct  CMPSS1_AC_BITS  bit;
};

struct CMPSS2_AC_BITS {                 
    Uint16 CPU1_ACC:2;                  
    Uint16 CLA1_ACC:2;                  
    Uint16 DMA1_ACC:2;                  
    Uint16 HICA_ACC:2;                  
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union CMPSS2_AC_REG {
    Uint32  all;
    struct  CMPSS2_AC_BITS  bit;
};

struct CMPSS3_AC_BITS {                 
    Uint16 CPU1_ACC:2;                  
    Uint16 CLA1_ACC:2;                  
    Uint16 DMA1_ACC:2;                  
    Uint16 HICA_ACC:2;                  
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union CMPSS3_AC_REG {
    Uint32  all;
    struct  CMPSS3_AC_BITS  bit;
};

struct CMPSS4_AC_BITS {                 
    Uint16 CPU1_ACC:2;                  
    Uint16 CLA1_ACC:2;                  
    Uint16 DMA1_ACC:2;                  
    Uint16 HICA_ACC:2;                  
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union CMPSS4_AC_REG {
    Uint32  all;
    struct  CMPSS4_AC_BITS  bit;
};

struct DACA_AC_BITS {                   
    Uint16 CPU1_ACC:2;                  
    Uint16 CLA1_ACC:2;                  
    Uint16 DMA1_ACC:2;                  
    Uint16 HICA_ACC:2;                  
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union DACA_AC_REG {
    Uint32  all;
    struct  DACA_AC_BITS  bit;
};

struct DACB_AC_BITS {                   
    Uint16 CPU1_ACC:2;                  
    Uint16 CLA1_ACC:2;                  
    Uint16 DMA1_ACC:2;                  
    Uint16 HICA_ACC:2;                  
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union DACB_AC_REG {
    Uint32  all;
    struct  DACB_AC_BITS  bit;
};

struct EPWM1_AC_BITS {                  
    Uint16 CPU1_ACC:2;                  
    Uint16 CLA1_ACC:2;                  
    Uint16 DMA1_ACC:2;                  
    Uint16 HICA_ACC:2;                  
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union EPWM1_AC_REG {
    Uint32  all;
    struct  EPWM1_AC_BITS  bit;
};

struct EPWM2_AC_BITS {                  
    Uint16 CPU1_ACC:2;                  
    Uint16 CLA1_ACC:2;                  
    Uint16 DMA1_ACC:2;                  
    Uint16 HICA_ACC:2;                  
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union EPWM2_AC_REG {
    Uint32  all;
    struct  EPWM2_AC_BITS  bit;
};

struct EPWM3_AC_BITS {                  
    Uint16 CPU1_ACC:2;                  
    Uint16 CLA1_ACC:2;                  
    Uint16 DMA1_ACC:2;                  
    Uint16 HICA_ACC:2;                  
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union EPWM3_AC_REG {
    Uint32  all;
    struct  EPWM3_AC_BITS  bit;
};

struct EPWM4_AC_BITS {                  
    Uint16 CPU1_ACC:2;                  
    Uint16 CLA1_ACC:2;                  
    Uint16 DMA1_ACC:2;                  
    Uint16 HICA_ACC:2;                  
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union EPWM4_AC_REG {
    Uint32  all;
    struct  EPWM4_AC_BITS  bit;
};

struct EPWM5_AC_BITS {                  
    Uint16 CPU1_ACC:2;                  
    Uint16 CLA1_ACC:2;                  
    Uint16 DMA1_ACC:2;                  
    Uint16 HICA_ACC:2;                  
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union EPWM5_AC_REG {
    Uint32  all;
    struct  EPWM5_AC_BITS  bit;
};

struct EPWM6_AC_BITS {                  
    Uint16 CPU1_ACC:2;                  
    Uint16 CLA1_ACC:2;                  
    Uint16 DMA1_ACC:2;                  
    Uint16 HICA_ACC:2;                  
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union EPWM6_AC_REG {
    Uint32  all;
    struct  EPWM6_AC_BITS  bit;
};

struct EPWM7_AC_BITS {                  
    Uint16 CPU1_ACC:2;                  
    Uint16 CLA1_ACC:2;                  
    Uint16 DMA1_ACC:2;                  
    Uint16 HICA_ACC:2;                  
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union EPWM7_AC_REG {
    Uint32  all;
    struct  EPWM7_AC_BITS  bit;
};

struct EPWM8_AC_BITS {                  
    Uint16 CPU1_ACC:2;                  
    Uint16 CLA1_ACC:2;                  
    Uint16 DMA1_ACC:2;                  
    Uint16 HICA_ACC:2;                  
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union EPWM8_AC_REG {
    Uint32  all;
    struct  EPWM8_AC_BITS  bit;
};

struct EQEP1_AC_BITS {                  
    Uint16 CPU1_ACC:2;                  
    Uint16 CLA1_ACC:2;                  
    Uint16 DMA1_ACC:2;                  
    Uint16 HICA_ACC:2;                  
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union EQEP1_AC_REG {
    Uint32  all;
    struct  EQEP1_AC_BITS  bit;
};

struct EQEP2_AC_BITS {                  
    Uint16 CPU1_ACC:2;                  
    Uint16 CLA1_ACC:2;                  
    Uint16 DMA1_ACC:2;                  
    Uint16 HICA_ACC:2;                  
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union EQEP2_AC_REG {
    Uint32  all;
    struct  EQEP2_AC_BITS  bit;
};

struct ECAP1_AC_BITS {                  
    Uint16 CPU1_ACC:2;                  
    Uint16 CLA1_ACC:2;                  
    Uint16 DMA1_ACC:2;                  
    Uint16 HICA_ACC:2;                  
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union ECAP1_AC_REG {
    Uint32  all;
    struct  ECAP1_AC_BITS  bit;
};

struct ECAP2_AC_BITS {                  
    Uint16 CPU1_ACC:2;                  
    Uint16 CLA1_ACC:2;                  
    Uint16 DMA1_ACC:2;                  
    Uint16 HICA_ACC:2;                  
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union ECAP2_AC_REG {
    Uint32  all;
    struct  ECAP2_AC_BITS  bit;
};

struct ECAP3_AC_BITS {                  
    Uint16 CPU1_ACC:2;                  
    Uint16 CLA1_ACC:2;                  
    Uint16 DMA1_ACC:2;                  
    Uint16 HICA_ACC:2;                  
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union ECAP3_AC_REG {
    Uint32  all;
    struct  ECAP3_AC_BITS  bit;
};

struct SDFM1_AC_BITS {                  
    Uint16 CPU1_ACC:2;                  
    Uint16 CLA1_ACC:2;                  
    Uint16 DMA1_ACC:2;                  
    Uint16 HICA_ACC:2;                  
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union SDFM1_AC_REG {
    Uint32  all;
    struct  SDFM1_AC_BITS  bit;
};

struct SDFM2_AC_BITS {                  
    Uint16 CPU1_ACC:2;                  
    Uint16 CLA1_ACC:2;                  
    Uint16 DMA1_ACC:2;                  
    Uint16 HICA_ACC:2;                  
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union SDFM2_AC_REG {
    Uint32  all;
    struct  SDFM2_AC_BITS  bit;
};

struct CLB1_AC_BITS {                   
    Uint16 CPU1_ACC:2;                  
    Uint16 CLA1_ACC:2;                  
    Uint16 rsvd1:2;                     
    Uint16 HICA_ACC:2;                  
    Uint16 rsvd2:8;                     
    Uint16 rsvd3:16;                    
};

union CLB1_AC_REG {
    Uint32  all;
    struct  CLB1_AC_BITS  bit;
};

struct CLB2_AC_BITS {                   
    Uint16 CPU1_ACC:2;                  
    Uint16 CLA1_ACC:2;                  
    Uint16 rsvd1:2;                     
    Uint16 HICA_ACC:2;                  
    Uint16 rsvd2:8;                     
    Uint16 rsvd3:16;                    
};

union CLB2_AC_REG {
    Uint32  all;
    struct  CLB2_AC_BITS  bit;
};

struct CLB3_AC_BITS {                   
    Uint16 CPU1_ACC:2;                  
    Uint16 CLA1_ACC:2;                  
    Uint16 rsvd1:2;                     
    Uint16 HICA_ACC:2;                  
    Uint16 rsvd2:8;                     
    Uint16 rsvd3:16;                    
};

union CLB3_AC_REG {
    Uint32  all;
    struct  CLB3_AC_BITS  bit;
};

struct CLB4_AC_BITS {                   
    Uint16 CPU1_ACC:2;                  
    Uint16 CLA1_ACC:2;                  
    Uint16 rsvd1:2;                     
    Uint16 HICA_ACC:2;                  
    Uint16 rsvd2:8;                     
    Uint16 rsvd3:16;                    
};

union CLB4_AC_REG {
    Uint32  all;
    struct  CLB4_AC_BITS  bit;
};

struct SCIA_AC_BITS {                   
    Uint16 CPU1_ACC:2;                  
    Uint16 rsvd1:2;                     
    Uint16 rsvd2:2;                     
    Uint16 HICA_ACC:2;                  
    Uint16 rsvd3:8;                     
    Uint16 rsvd4:16;                    
};

union SCIA_AC_REG {
    Uint32  all;
    struct  SCIA_AC_BITS  bit;
};

struct SCIB_AC_BITS {                   
    Uint16 CPU1_ACC:2;                  
    Uint16 rsvd1:2;                     
    Uint16 rsvd2:2;                     
    Uint16 HICA_ACC:2;                  
    Uint16 rsvd3:8;                     
    Uint16 rsvd4:16;                    
};

union SCIB_AC_REG {
    Uint32  all;
    struct  SCIB_AC_BITS  bit;
};

struct SPIA_AC_BITS {                   
    Uint16 CPU1_ACC:2;                  
    Uint16 CLA1_ACC:2;                  
    Uint16 DMA1_ACC:2;                  
    Uint16 HICA_ACC:2;                  
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union SPIA_AC_REG {
    Uint32  all;
    struct  SPIA_AC_BITS  bit;
};

struct SPIB_AC_BITS {                   
    Uint16 CPU1_ACC:2;                  
    Uint16 CLA1_ACC:2;                  
    Uint16 DMA1_ACC:2;                  
    Uint16 HICA_ACC:2;                  
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union SPIB_AC_REG {
    Uint32  all;
    struct  SPIB_AC_BITS  bit;
};

struct I2CA_AC_BITS {                   
    Uint16 CPU1_ACC:2;                  
    Uint16 rsvd1:2;                     
    Uint16 rsvd2:2;                     
    Uint16 HICA_ACC:2;                  
    Uint16 rsvd3:8;                     
    Uint16 rsvd4:16;                    
};

union I2CA_AC_REG {
    Uint32  all;
    struct  I2CA_AC_BITS  bit;
};

struct I2CB_AC_BITS {                   
    Uint16 CPU1_ACC:2;                  
    Uint16 rsvd1:2;                     
    Uint16 rsvd2:2;                     
    Uint16 HICA_ACC:2;                  
    Uint16 rsvd3:8;                     
    Uint16 rsvd4:16;                    
};

union I2CB_AC_REG {
    Uint32  all;
    struct  I2CB_AC_BITS  bit;
};

struct PMBUS_A_AC_BITS {                
    Uint16 CPU1_ACC:2;                  
    Uint16 CLA1_ACC:2;                  
    Uint16 DMA1_ACC:2;                  
    Uint16 HICA_ACC:2;                  
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union PMBUS_A_AC_REG {
    Uint32  all;
    struct  PMBUS_A_AC_BITS  bit;
};

struct LIN_A_AC_BITS {                  
    Uint16 CPU1_ACC:2;                  
    Uint16 CLA1_ACC:2;                  
    Uint16 DMA1_ACC:2;                  
    Uint16 HICA_ACC:2;                  
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union LIN_A_AC_REG {
    Uint32  all;
    struct  LIN_A_AC_BITS  bit;
};

struct LIN_B_AC_BITS {                  
    Uint16 CPU1_ACC:2;                  
    Uint16 CLA1_ACC:2;                  
    Uint16 DMA1_ACC:2;                  
    Uint16 HICA_ACC:2;                  
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union LIN_B_AC_REG {
    Uint32  all;
    struct  LIN_B_AC_BITS  bit;
};

struct DCANA_AC_BITS {                  
    Uint16 CPU1_ACC:2;                  
    Uint16 rsvd1:2;                     
    Uint16 DMA1_ACC:2;                  
    Uint16 HICA_ACC:2;                  
    Uint16 rsvd2:8;                     
    Uint16 rsvd3:16;                    
};

union DCANA_AC_REG {
    Uint32  all;
    struct  DCANA_AC_BITS  bit;
};

struct MCANA_AC_BITS {                  
    Uint16 CPU1_ACC:2;                  
    Uint16 rsvd1:2;                     
    Uint16 rsvd2:2;                     
    Uint16 HICA_ACC:2;                  
    Uint16 rsvd3:8;                     
    Uint16 rsvd4:16;                    
};

union MCANA_AC_REG {
    Uint32  all;
    struct  MCANA_AC_BITS  bit;
};

struct FSIATX_AC_BITS {                 
    Uint16 CPU1_ACC:2;                  
    Uint16 CLA1_ACC:2;                  
    Uint16 DMA1_ACC:2;                  
    Uint16 HICA_ACC:2;                  
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union FSIATX_AC_REG {
    Uint32  all;
    struct  FSIATX_AC_BITS  bit;
};

struct FSIARX_AC_BITS {                 
    Uint16 CPU1_ACC:2;                  
    Uint16 CLA1_ACC:2;                  
    Uint16 DMA1_ACC:2;                  
    Uint16 HICA_ACC:2;                  
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union FSIARX_AC_REG {
    Uint32  all;
    struct  FSIARX_AC_BITS  bit;
};

struct HRPWM_A_AC_BITS {                
    Uint16 CPU1_ACC:2;                  
    Uint16 CLA1_ACC:2;                  
    Uint16 DMA1_ACC:2;                  
    Uint16 HICA_ACC:2;                  
    Uint16 rsvd1:8;                     
    Uint16 rsvd2:16;                    
};

union HRPWM_A_AC_REG {
    Uint32  all;
    struct  HRPWM_A_AC_BITS  bit;
};

struct HIC_A_AC_BITS {                  
    Uint16 CPU1_ACC:2;                  
    Uint16 rsvd1:2;                     
    Uint16 DMA1_ACC:2;                  
    Uint16 rsvd2:2;                     
    Uint16 rsvd3:8;                     
    Uint16 rsvd4:16;                    
};

union HIC_A_AC_REG {
    Uint32  all;
    struct  HIC_A_AC_BITS  bit;
};

struct AESA_AC_BITS {                   
    Uint16 CPU1_ACC:2;                  
    Uint16 rsvd1:2;                     
    Uint16 DMA1_ACC:2;                  
    Uint16 rsvd2:2;                     
    Uint16 rsvd3:8;                     
    Uint16 rsvd4:16;                    
};

union AESA_AC_REG {
    Uint32  all;
    struct  AESA_AC_BITS  bit;
};

struct PERIPH_AC_LOCK_BITS {            
    Uint16 LOCK_AC_WR:1;                
    Uint16 rsvd1:15;                    
    Uint16 rsvd2:16;                    
};

union PERIPH_AC_LOCK_REG {
    Uint32  all;
    struct  PERIPH_AC_LOCK_BITS  bit;
};

struct PERIPH_AC_REGS {
    union   ADCA_AC_REG                      ADCA_AC;                      
    union   ADCB_AC_REG                      ADCB_AC;                      
    union   ADCC_AC_REG                      ADCC_AC;                      
    Uint16                                   rsvd1[10];                    
    union   CMPSS1_AC_REG                    CMPSS1_AC;                    
    union   CMPSS2_AC_REG                    CMPSS2_AC;                    
    union   CMPSS3_AC_REG                    CMPSS3_AC;                    
    union   CMPSS4_AC_REG                    CMPSS4_AC;                    
    Uint16                                   rsvd2[16];                    
    union   DACA_AC_REG                      DACA_AC;                      
    union   DACB_AC_REG                      DACB_AC;                      
    Uint16                                   rsvd3[28];                    
    union   EPWM1_AC_REG                     EPWM1_AC;                     
    union   EPWM2_AC_REG                     EPWM2_AC;                     
    union   EPWM3_AC_REG                     EPWM3_AC;                     
    union   EPWM4_AC_REG                     EPWM4_AC;                     
    union   EPWM5_AC_REG                     EPWM5_AC;                     
    union   EPWM6_AC_REG                     EPWM6_AC;                     
    union   EPWM7_AC_REG                     EPWM7_AC;                     
    union   EPWM8_AC_REG                     EPWM8_AC;                     
    Uint16                                   rsvd4[24];                    
    union   EQEP1_AC_REG                     EQEP1_AC;                     
    union   EQEP2_AC_REG                     EQEP2_AC;                     
    Uint16                                   rsvd5[12];                    
    union   ECAP1_AC_REG                     ECAP1_AC;                     
    union   ECAP2_AC_REG                     ECAP2_AC;                     
    union   ECAP3_AC_REG                     ECAP3_AC;                     
    Uint16                                   rsvd6[34];                    
    union   SDFM1_AC_REG                     SDFM1_AC;                     
    union   SDFM2_AC_REG                     SDFM2_AC;                     
    Uint16                                   rsvd7[4];                     
    union   CLB1_AC_REG                      CLB1_AC;                      
    union   CLB2_AC_REG                      CLB2_AC;                      
    union   CLB3_AC_REG                      CLB3_AC;                      
    union   CLB4_AC_REG                      CLB4_AC;                      
    Uint16                                   rsvd8[72];                    
    union   SCIA_AC_REG                      SCIA_AC;                      
    union   SCIB_AC_REG                      SCIB_AC;                      
    Uint16                                   rsvd9[12];                    
    union   SPIA_AC_REG                      SPIA_AC;                      
    union   SPIB_AC_REG                      SPIB_AC;                      
    Uint16                                   rsvd10[12];                   
    union   I2CA_AC_REG                      I2CA_AC;                      
    union   I2CB_AC_REG                      I2CB_AC;                      
    Uint16                                   rsvd11[12];                   
    union   PMBUS_A_AC_REG                   PMBUS_A_AC;                   
    Uint16                                   rsvd12[6];                    
    union   LIN_A_AC_REG                     LIN_A_AC;                     
    union   LIN_B_AC_REG                     LIN_B_AC;                     
    Uint16                                   rsvd13[4];                    
    union   DCANA_AC_REG                     DCANA_AC;                     
    Uint16                                   rsvd14[6];                    
    union   MCANA_AC_REG                     MCANA_AC;                     
    Uint16                                   rsvd15[14];                   
    union   FSIATX_AC_REG                    FSIATX_AC;                    
    union   FSIARX_AC_REG                    FSIARX_AC;                    
    Uint16                                   rsvd16[78];                   
    union   HRPWM_A_AC_REG                   HRPWM_A_AC;                   
    union   HIC_A_AC_REG                     HIC_A_AC;                     
    union   AESA_AC_REG                      AESA_AC;                      
    Uint16                                   rsvd17[78];                   
    union   PERIPH_AC_LOCK_REG               PERIPH_AC_LOCK;               
};

struct SYNCSELECT_BITS {                
    Uint16 rsvd1:3;                     
    Uint16 rsvd2:3;                     
    Uint16 rsvd3:3;                     
    Uint16 rsvd4:3;                     
    Uint16 rsvd5:3;                     
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:2;                     
    Uint16 rsvd8:6;                     
    Uint16 SYNCOUT:5;                   
    Uint16 rsvd9:3;                     
};

union SYNCSELECT_REG {
    Uint32  all;
    struct  SYNCSELECT_BITS  bit;
};

struct ADCSOCOUTSELECT_BITS {           
    Uint16 PWM1SOCAEN:1;                
    Uint16 PWM2SOCAEN:1;                
    Uint16 PWM3SOCAEN:1;                
    Uint16 PWM4SOCAEN:1;                
    Uint16 PWM5SOCAEN:1;                
    Uint16 PWM6SOCAEN:1;                
    Uint16 PWM7SOCAEN:1;                
    Uint16 PWM8SOCAEN:1;                
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:4;                     
    Uint16 PWM1SOCBEN:1;                
    Uint16 PWM2SOCBEN:1;                
    Uint16 PWM3SOCBEN:1;                
    Uint16 PWM4SOCBEN:1;                
    Uint16 PWM5SOCBEN:1;                
    Uint16 PWM6SOCBEN:1;                
    Uint16 PWM7SOCBEN:1;                
    Uint16 PWM8SOCBEN:1;                
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:1;                     
    Uint16 rsvd8:1;                     
    Uint16 rsvd9:1;                     
    Uint16 rsvd10:4;                    
};

union ADCSOCOUTSELECT_REG {
    Uint32  all;
    struct  ADCSOCOUTSELECT_BITS  bit;
};

struct SYNCSOCLOCK_BITS {               
    Uint16 SYNCSELECT:1;                
    Uint16 ADCSOCOUTSELECT:1;           
    Uint16 rsvd1:14;                    
    Uint16 rsvd2:16;                    
};

union SYNCSOCLOCK_REG {
    Uint32  all;
    struct  SYNCSOCLOCK_BITS  bit;
};

struct SYNC_SOC_REGS {
    union   SYNCSELECT_REG                   SYNCSELECT;                   
    union   ADCSOCOUTSELECT_REG              ADCSOCOUTSELECT;              
    union   SYNCSOCLOCK_REG                  SYNCSOCLOCK;                  
};

struct LFUConfig_BITS {                 
    Uint16 LFU_CPU:1;                   
    Uint16 rsvd1:3;                     
    Uint16 LFU_CLA1:1;                  
    Uint16 rsvd2:3;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:3;                     
    Uint16 PieVectorSwap:1;             
    Uint16 rsvd5:3;                     
    Uint16 LS01Swap:1;                  
    Uint16 rsvd6:15;                    
};

union LFUConfig_REG {
    Uint32  all;
    struct  LFUConfig_BITS  bit;
};

struct LFUStatus_BITS {                 
    Uint16 rsvd1:12;                    
    Uint16 PieVectorSwap:1;             
    Uint16 rsvd2:3;                     
    Uint16 LS01Swap:1;                  
    Uint16 rsvd3:15;                    
};

union LFUStatus_REG {
    Uint32  all;
    struct  LFUStatus_BITS  bit;
};

struct LFU_LOCK_BITS {                  
    Uint16 LFUConfig:1;                 
    Uint16 rsvd1:7;                     
    Uint16 SWConfig1_SYSRSn:1;          
    Uint16 SWConfig2_SYSRSn:1;          
    Uint16 SWConfig1_XRSn:1;            
    Uint16 SWConfig2_XRSn:1;            
    Uint16 SWConfig1_PORESETn:1;        
    Uint16 SWConfig2_PORESETn:1;        
    Uint16 rsvd2:2;                     
    Uint16 rsvd3:16;                    
};

union LFU_LOCK_REG {
    Uint32  all;
    struct  LFU_LOCK_BITS  bit;
};

struct LFU_COMMIT_BITS {                
    Uint16 LFUConfig:1;                 
    Uint16 rsvd1:7;                     
    Uint16 SWConfig1_SYSRSn:1;          
    Uint16 SWConfig2_SYSRSn:1;          
    Uint16 SWConfig1_XRSn:1;            
    Uint16 SWConfig2_XRSn:1;            
    Uint16 SWConfig1_PORESETn:1;        
    Uint16 SWConfig2_PORESETn:1;        
    Uint16 rsvd2:2;                     
    Uint16 rsvd3:16;                    
};

union LFU_COMMIT_REG {
    Uint32  all;
    struct  LFU_COMMIT_BITS  bit;
};

struct LFU_REGS {
    union   LFUConfig_REG                    LFUConfig;                    
    union   LFUStatus_REG                    LFUStatus;                    
    Uint16                                   rsvd1[12];                    
    Uint32                                   SWConfig1_SYSRSn;             
    Uint32                                   SWConfig2_SYSRSn;             
    Uint32                                   SWConfig1_XRSn;               
    Uint32                                   SWConfig2_XRSn;               
    Uint32                                   SWConfig1_PORESETn;           
    Uint32                                   SWConfig2_PORESETn;           
    union   LFU_LOCK_REG                     LFU_LOCK;                     
    union   LFU_COMMIT_REG                   LFU_COMMIT;                   
};

struct SYS_ERR_INT_FLG_BITS {           
    Uint16 GINT:1;                      
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:1;                     
    Uint16 rsvd8:1;                     
    Uint16 rsvd9:1;                     
    Uint16 AES_BUS_ERROR:1;             
    Uint16 EPG1_INT:1;                  
    Uint16 rsvd10:1;                    
    Uint16 rsvd11:1;                    
    Uint16 rsvd12:1;                    
    Uint16 rsvd13:1;                    
    Uint16 rsvd14:16;                   
};

union SYS_ERR_INT_FLG_REG {
    Uint32  all;
    struct  SYS_ERR_INT_FLG_BITS  bit;
};

struct SYS_ERR_INT_CLR_BITS {           
    Uint16 GINT:1;                      
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:1;                     
    Uint16 rsvd8:1;                     
    Uint16 rsvd9:1;                     
    Uint16 AES_BUS_ERROR:1;             
    Uint16 EPG1_INT:1;                  
    Uint16 rsvd10:1;                    
    Uint16 rsvd11:1;                    
    Uint16 rsvd12:1;                    
    Uint16 rsvd13:1;                    
    Uint16 rsvd14:16;                   
};

union SYS_ERR_INT_CLR_REG {
    Uint32  all;
    struct  SYS_ERR_INT_CLR_BITS  bit;
};

struct SYS_ERR_INT_SET_BITS {           
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:1;                     
    Uint16 rsvd8:1;                     
    Uint16 rsvd9:1;                     
    Uint16 rsvd10:1;                    
    Uint16 AES_BUS_ERROR:1;             
    Uint16 EPG1_INT:1;                  
    Uint16 rsvd11:1;                    
    Uint16 rsvd12:1;                    
    Uint16 rsvd13:1;                    
    Uint16 rsvd14:1;                    
    Uint16 rsvd15:8;                    
    Uint16 KEY:8;                       
};

union SYS_ERR_INT_SET_REG {
    Uint32  all;
    struct  SYS_ERR_INT_SET_BITS  bit;
};

struct SYS_ERR_MASK_BITS {              
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:1;                     
    Uint16 rsvd8:1;                     
    Uint16 rsvd9:1;                     
    Uint16 rsvd10:1;                    
    Uint16 AES_BUS_ERROR:1;             
    Uint16 EPG1_INT:1;                  
    Uint16 rsvd11:1;                    
    Uint16 rsvd12:1;                    
    Uint16 rsvd13:1;                    
    Uint16 rsvd14:1;                    
    Uint16 rsvd15:8;                    
    Uint16 KEY:8;                       
};

union SYS_ERR_MASK_REG {
    Uint32  all;
    struct  SYS_ERR_MASK_BITS  bit;
};

struct SYS_STATUS_REGS {
    Uint16                                   rsvd1[16];                    
    union   SYS_ERR_INT_FLG_REG              SYS_ERR_INT_FLG;              
    union   SYS_ERR_INT_CLR_REG              SYS_ERR_INT_CLR;              
    union   SYS_ERR_INT_SET_REG              SYS_ERR_INT_SET;              
    union   SYS_ERR_MASK_REG                 SYS_ERR_MASK;                 
};




extern volatile struct SYNC_SOC_REGS SyncSocRegs;
extern volatile struct DMA_CLA_SRC_SEL_REGS DmaClaSrcSelRegs;
extern volatile struct LFU_REGS LfuRegs;
extern volatile struct DEV_CFG_REGS DevCfgRegs;
extern volatile struct CLK_CFG_REGS ClkCfgRegs;
extern volatile struct CPU_SYS_REGS CpuSysRegs;
extern volatile struct SYS_STATUS_REGS SysStatusRegs;
extern volatile struct PERIPH_AC_REGS PeriphAcRegs;



















































struct SCSR_BITS {                      
    Uint16 WDOVERRIDE:1;                
    Uint16 WDENINT:1;                   
    Uint16 WDINTS:1;                    
    Uint16 rsvd1:13;                    
};

union SCSR_REG {
    Uint16  all;
    struct  SCSR_BITS  bit;
};

struct WDCNTR_BITS {                    
    Uint16 WDCNTR:8;                    
    Uint16 rsvd1:8;                     
};

union WDCNTR_REG {
    Uint16  all;
    struct  WDCNTR_BITS  bit;
};

struct WDKEY_BITS {                     
    Uint16 WDKEY:8;                     
    Uint16 rsvd1:8;                     
};

union WDKEY_REG {
    Uint16  all;
    struct  WDKEY_BITS  bit;
};

struct WDCR_BITS {                      
    Uint16 WDPS:3;                      
    Uint16 WDCHK:3;                     
    Uint16 WDDIS:1;                     
    Uint16 WDFLG:1;                     
    Uint16 WDPRECLKDIV:4;               
    Uint16 rsvd1:4;                     
};

union WDCR_REG {
    Uint16  all;
    struct  WDCR_BITS  bit;
};

struct WDWCR_BITS {                     
    Uint16 MIN:8;                       
    Uint16 FIRSTKEY:1;                  
    Uint16 rsvd1:7;                     
};

union WDWCR_REG {
    Uint16  all;
    struct  WDWCR_BITS  bit;
};

struct WD_REGS {
    Uint16                                   rsvd1[34];                    
    union   SCSR_REG                         SCSR;                         
    union   WDCNTR_REG                       WDCNTR;                       
    Uint16                                   rsvd2;                        
    union   WDKEY_REG                        WDKEY;                        
    Uint16                                   rsvd3[3];                     
    union   WDCR_REG                         WDCR;                         
    union   WDWCR_REG                        WDWCR;                        
};




extern volatile struct WD_REGS WdRegs;



















































struct XBARFLG1_BITS {                  
    Uint16 CMPSS1_CTRIPL:1;             
    Uint16 CMPSS1_CTRIPH:1;             
    Uint16 CMPSS2_CTRIPL:1;             
    Uint16 CMPSS2_CTRIPH:1;             
    Uint16 CMPSS3_CTRIPL:1;             
    Uint16 CMPSS3_CTRIPH:1;             
    Uint16 CMPSS4_CTRIPL:1;             
    Uint16 CMPSS4_CTRIPH:1;             
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:1;                     
    Uint16 rsvd8:1;                     
    Uint16 CMPSS1_CTRIPOUTL:1;          
    Uint16 CMPSS1_CTRIPOUTH:1;          
    Uint16 CMPSS2_CTRIPOUTL:1;          
    Uint16 CMPSS2_CTRIPOUTH:1;          
    Uint16 CMPSS3_CTRIPOUTL:1;          
    Uint16 CMPSS3_CTRIPOUTH:1;          
    Uint16 CMPSS4_CTRIPOUTL:1;          
    Uint16 CMPSS4_CTRIPOUTH:1;          
    Uint16 rsvd9:1;                     
    Uint16 rsvd10:1;                    
    Uint16 rsvd11:1;                    
    Uint16 rsvd12:1;                    
    Uint16 rsvd13:1;                    
    Uint16 rsvd14:1;                    
    Uint16 rsvd15:1;                    
    Uint16 rsvd16:1;                    
};

union XBARFLG1_REG {
    Uint32  all;
    struct  XBARFLG1_BITS  bit;
};

struct XBARFLG2_BITS {                  
    Uint16 INPUT1:1;                    
    Uint16 INPUT2:1;                    
    Uint16 INPUT3:1;                    
    Uint16 INPUT4:1;                    
    Uint16 INPUT5:1;                    
    Uint16 INPUT6:1;                    
    Uint16 ADCSOCA:1;                   
    Uint16 ADCSOCB:1;                   
    Uint16 INPUT7:1;                    
    Uint16 INPUT8:1;                    
    Uint16 INPUT9:1;                    
    Uint16 INPUT10:1;                   
    Uint16 INPUT11:1;                   
    Uint16 INPUT12:1;                   
    Uint16 INPUT13:1;                   
    Uint16 INPUT14:1;                   
    Uint16 ECAP1_OUT:1;                 
    Uint16 ECAP2_OUT:1;                 
    Uint16 ECAP3_OUT:1;                 
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 EXTSYNCOUT:1;                
    Uint16 ADCAEVT1:1;                  
    Uint16 ADCAEVT2:1;                  
    Uint16 ADCAEVT3:1;                  
    Uint16 ADCAEVT4:1;                  
    Uint16 ADCBEVT1:1;                  
    Uint16 ADCBEVT2:1;                  
    Uint16 ADCBEVT3:1;                  
    Uint16 ADCBEVT4:1;                  
    Uint16 ADCCEVT1:1;                  
};

union XBARFLG2_REG {
    Uint32  all;
    struct  XBARFLG2_BITS  bit;
};

struct XBARFLG3_BITS {                  
    Uint16 ADCCEVT2:1;                  
    Uint16 ADCCEVT3:1;                  
    Uint16 ADCCEVT4:1;                  
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 SD1FLT1_COMPL:1;             
    Uint16 SD1FLT1_COMPH:1;             
    Uint16 SD1FLT2_COMPL:1;             
    Uint16 SD1FLT2_COMPH:1;             
    Uint16 SD1FLT3_COMPL:1;             
    Uint16 SD1FLT3_COMPH:1;             
    Uint16 SD1FLT4_COMPL:1;             
    Uint16 SD1FLT4_COMPH:1;             
    Uint16 SD2FLT1_COMPL:1;             
    Uint16 SD2FLT1_COMPH:1;             
    Uint16 SD2FLT2_COMPL:1;             
    Uint16 SD2FLT2_COMPH:1;             
    Uint16 SD2FLT3_COMPL:1;             
    Uint16 SD2FLT3_COMPH:1;             
    Uint16 SD2FLT4_COMPL:1;             
    Uint16 SD2FLT4_COMPH:1;             
    Uint16 rsvd5:1;                     
    Uint16 SD1FLT1_COMPZ:1;             
    Uint16 SD1FLT1_DRINT:1;             
    Uint16 SD1FLT2_COMPZ:1;             
    Uint16 SD1FLT2_DRINT:1;             
    Uint16 SD1FLT3_COMPZ:1;             
    Uint16 SD1FLT3_DRINT:1;             
    Uint16 SD1FLT4_COMPZ:1;             
    Uint16 SD1FLT4_DRINT:1;             
};

union XBARFLG3_REG {
    Uint32  all;
    struct  XBARFLG3_BITS  bit;
};

struct XBARFLG4_BITS {                  
    Uint16 SD2FLT1_COMPZ:1;             
    Uint16 SD2FLT1_DRINT:1;             
    Uint16 SD2FLT2_COMPZ:1;             
    Uint16 SD2FLT2_DRINT:1;             
    Uint16 SD2FLT3_COMPZ:1;             
    Uint16 SD2FLT3_DRINT:1;             
    Uint16 SD2FLT4_COMPZ:1;             
    Uint16 SD2FLT4_DRINT:1;             
    Uint16 rsvd1:1;                     
    Uint16 MCANA_FEVT0:1;               
    Uint16 MCANA_FEVT1:1;               
    Uint16 MCANA_FEVT2:1;               
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 CLB1_4_1:1;                  
    Uint16 CLB1_5_1:1;                  
    Uint16 CLB2_4_1:1;                  
    Uint16 CLB2_5_1:1;                  
    Uint16 CLB3_4_1:1;                  
    Uint16 CLB3_5_1:1;                  
    Uint16 CLB4_4_1:1;                  
    Uint16 CLB4_5_1:1;                  
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:1;                     
    Uint16 rsvd8:1;                     
    Uint16 rsvd9:1;                     
    Uint16 ERRORSTS_ERROR:1;            
    Uint16 rsvd10:1;                    
    Uint16 rsvd11:1;                    
    Uint16 CLAHALT:1;                   
};

union XBARFLG4_REG {
    Uint32  all;
    struct  XBARFLG4_BITS  bit;
};

struct XBARCLR1_BITS {                  
    Uint16 CMPSS1_CTRIPL:1;             
    Uint16 CMPSS1_CTRIPH:1;             
    Uint16 CMPSS2_CTRIPL:1;             
    Uint16 CMPSS2_CTRIPH:1;             
    Uint16 CMPSS3_CTRIPL:1;             
    Uint16 CMPSS3_CTRIPH:1;             
    Uint16 CMPSS4_CTRIPL:1;             
    Uint16 CMPSS4_CTRIPH:1;             
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:1;                     
    Uint16 rsvd8:1;                     
    Uint16 CMPSS1_CTRIPOUTL:1;          
    Uint16 CMPSS1_CTRIPOUTH:1;          
    Uint16 CMPSS2_CTRIPOUTL:1;          
    Uint16 CMPSS2_CTRIPOUTH:1;          
    Uint16 CMPSS3_CTRIPOUTL:1;          
    Uint16 CMPSS3_CTRIPOUTH:1;          
    Uint16 CMPSS4_CTRIPOUTL:1;          
    Uint16 CMPSS4_CTRIPOUTH:1;          
    Uint16 rsvd9:1;                     
    Uint16 rsvd10:1;                    
    Uint16 rsvd11:1;                    
    Uint16 rsvd12:1;                    
    Uint16 rsvd13:1;                    
    Uint16 rsvd14:1;                    
    Uint16 rsvd15:1;                    
    Uint16 rsvd16:1;                    
};

union XBARCLR1_REG {
    Uint32  all;
    struct  XBARCLR1_BITS  bit;
};

struct XBARCLR2_BITS {                  
    Uint16 INPUT1:1;                    
    Uint16 INPUT2:1;                    
    Uint16 INPUT3:1;                    
    Uint16 INPUT4:1;                    
    Uint16 INPUT5:1;                    
    Uint16 INPUT6:1;                    
    Uint16 ADCSOCA:1;                   
    Uint16 ADCSOCB:1;                   
    Uint16 INPUT7:1;                    
    Uint16 INPUT8:1;                    
    Uint16 INPUT9:1;                    
    Uint16 INPUT10:1;                   
    Uint16 INPUT11:1;                   
    Uint16 INPUT12:1;                   
    Uint16 INPUT13:1;                   
    Uint16 INPUT14:1;                   
    Uint16 ECAP1_OUT:1;                 
    Uint16 ECAP2_OUT:1;                 
    Uint16 ECAP3_OUT:1;                 
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 EXTSYNCOUT:1;                
    Uint16 ADCAEVT1:1;                  
    Uint16 ADCAEVT2:1;                  
    Uint16 ADCAEVT3:1;                  
    Uint16 ADCAEVT4:1;                  
    Uint16 ADCBEVT1:1;                  
    Uint16 ADCBEVT2:1;                  
    Uint16 ADCBEVT3:1;                  
    Uint16 ADCBEVT4:1;                  
    Uint16 ADCCEVT1:1;                  
};

union XBARCLR2_REG {
    Uint32  all;
    struct  XBARCLR2_BITS  bit;
};

struct XBARCLR3_BITS {                  
    Uint16 ADCCEVT2:1;                  
    Uint16 ADCCEVT3:1;                  
    Uint16 ADCCEVT4:1;                  
    Uint16 rsvd1:1;                     
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 SD1FLT1_COMPL:1;             
    Uint16 SD1FLT1_COMPH:1;             
    Uint16 SD1FLT2_COMPL:1;             
    Uint16 SD1FLT2_COMPH:1;             
    Uint16 SD1FLT3_COMPL:1;             
    Uint16 SD1FLT3_COMPH:1;             
    Uint16 SD1FLT4_COMPL:1;             
    Uint16 SD1FLT4_COMPH:1;             
    Uint16 SD2FLT1_COMPL:1;             
    Uint16 SD2FLT1_COMPH:1;             
    Uint16 SD2FLT2_COMPL:1;             
    Uint16 SD2FLT2_COMPH:1;             
    Uint16 SD2FLT3_COMPL:1;             
    Uint16 SD2FLT3_COMPH:1;             
    Uint16 SD2FLT4_COMPL:1;             
    Uint16 SD2FLT4_COMPH:1;             
    Uint16 rsvd5:1;                     
    Uint16 SD1FLT1_COMPZ:1;             
    Uint16 SD1FLT1_DRINT:1;             
    Uint16 SD1FLT2_COMPZ:1;             
    Uint16 SD1FLT2_DRINT:1;             
    Uint16 SD1FLT3_COMPZ:1;             
    Uint16 SD1FLT3_DRINT:1;             
    Uint16 SD1FLT4_COMPZ:1;             
    Uint16 SD1FLT4_DRINT:1;             
};

union XBARCLR3_REG {
    Uint32  all;
    struct  XBARCLR3_BITS  bit;
};

struct XBARCLR4_BITS {                  
    Uint16 SD2FLT1_COMPZ:1;             
    Uint16 SD2FLT1_DRINT:1;             
    Uint16 SD2FLT2_COMPZ:1;             
    Uint16 SD2FLT2_DRINT:1;             
    Uint16 SD2FLT3_COMPZ:1;             
    Uint16 SD2FLT3_DRINT:1;             
    Uint16 SD2FLT4_COMPZ:1;             
    Uint16 SD2FLT4_DRINT:1;             
    Uint16 rsvd1:1;                     
    Uint16 MCANA_FEVT0:1;               
    Uint16 MCANA_FEVT1:1;               
    Uint16 MCANA_FEVT2:1;               
    Uint16 rsvd2:1;                     
    Uint16 rsvd3:1;                     
    Uint16 rsvd4:1;                     
    Uint16 rsvd5:1;                     
    Uint16 CLB1_4_1:1;                  
    Uint16 CLB1_5_1:1;                  
    Uint16 CLB2_4_1:1;                  
    Uint16 CLB2_5_1:1;                  
    Uint16 CLB3_4_1:1;                  
    Uint16 CLB3_5_1:1;                  
    Uint16 CLB4_4_1:1;                  
    Uint16 CLB4_5_1:1;                  
    Uint16 rsvd6:1;                     
    Uint16 rsvd7:1;                     
    Uint16 rsvd8:1;                     
    Uint16 rsvd9:1;                     
    Uint16 ERRORSTS_ERROR:1;            
    Uint16 rsvd10:1;                    
    Uint16 rsvd11:1;                    
    Uint16 CLAHALT:1;                   
};

union XBARCLR4_REG {
    Uint32  all;
    struct  XBARCLR4_BITS  bit;
};

struct XBAR_REGS {
    union   XBARFLG1_REG                     XBARFLG1;                     
    union   XBARFLG2_REG                     XBARFLG2;                     
    union   XBARFLG3_REG                     XBARFLG3;                     
    union   XBARFLG4_REG                     XBARFLG4;                     
    union   XBARCLR1_REG                     XBARCLR1;                     
    union   XBARCLR2_REG                     XBARCLR2;                     
    union   XBARCLR3_REG                     XBARCLR3;                     
    union   XBARCLR4_REG                     XBARCLR4;                     
};




extern volatile struct XBAR_REGS XbarRegs;



















































struct XINT1CR_BITS {                   
    Uint16 ENABLE:1;                    
    Uint16 rsvd1:1;                     
    Uint16 POLARITY:2;                  
    Uint16 rsvd2:12;                    
};

union XINT1CR_REG {
    Uint16  all;
    struct  XINT1CR_BITS  bit;
};

struct XINT2CR_BITS {                   
    Uint16 ENABLE:1;                    
    Uint16 rsvd1:1;                     
    Uint16 POLARITY:2;                  
    Uint16 rsvd2:12;                    
};

union XINT2CR_REG {
    Uint16  all;
    struct  XINT2CR_BITS  bit;
};

struct XINT3CR_BITS {                   
    Uint16 ENABLE:1;                    
    Uint16 rsvd1:1;                     
    Uint16 POLARITY:2;                  
    Uint16 rsvd2:12;                    
};

union XINT3CR_REG {
    Uint16  all;
    struct  XINT3CR_BITS  bit;
};

struct XINT4CR_BITS {                   
    Uint16 ENABLE:1;                    
    Uint16 rsvd1:1;                     
    Uint16 POLARITY:2;                  
    Uint16 rsvd2:12;                    
};

union XINT4CR_REG {
    Uint16  all;
    struct  XINT4CR_BITS  bit;
};

struct XINT5CR_BITS {                   
    Uint16 ENABLE:1;                    
    Uint16 rsvd1:1;                     
    Uint16 POLARITY:2;                  
    Uint16 rsvd2:12;                    
};

union XINT5CR_REG {
    Uint16  all;
    struct  XINT5CR_BITS  bit;
};

struct XINT_REGS {
    union   XINT1CR_REG                      XINT1CR;                      
    union   XINT2CR_REG                      XINT2CR;                      
    union   XINT3CR_REG                      XINT3CR;                      
    union   XINT4CR_REG                      XINT4CR;                      
    union   XINT5CR_REG                      XINT5CR;                      
    Uint16                                   rsvd1[3];                     
    Uint16                                   XINT1CTR;                     
    Uint16                                   XINT2CTR;                     
    Uint16                                   XINT3CTR;                     
};




extern volatile struct XINT_REGS XintRegs;

















































void InitTempSensor(float vrefhi_voltage);
int16_t GetTemperatureC(uint16_t sensorSample);
int16_t GetTemperatureK(uint16_t sensorSample);






















































struct CAN_CTL_BITS {                   
    bp_16 Init:1;                       
    bp_16 IE0:1;                        
    bp_16 SIE:1;                        
    bp_16 EIE:1;                        
    bp_16 rsvd1:1;                      
    bp_16 DAR:1;                        
    bp_16 CCE:1;                        
    bp_16 Test:1;                       
    bp_16 IDS:1;                        
    bp_16 ABO:1;                        
    bp_16 PMD:4;                        
    bp_16 rsvd2:1;                      
    bp_16 SWR:1;                        
    bp_32 INITDBG:1;                    
    bp_32 IE1:1;                        
    bp_32 DE1:1;                        
    bp_32 DE2:1;                        
    bp_32 DE3:1;                        
    bp_32 rsvd3:3;                      
    bp_32 rsvd4:1;                      
    bp_32 rsvd5:1;                      
    bp_32 rsvd6:6;                      
};

union CAN_CTL_REG {
    bp_32  all;
    struct  CAN_CTL_BITS  bit;
};

struct CAN_ES_BITS {                    
    bp_16 LEC:3;                        
    bp_16 TxOk:1;                       
    bp_16 RxOk:1;                       
    bp_16 EPass:1;                      
    bp_16 EWarn:1;                      
    bp_16 BOff:1;                       
    bp_16 PER:1;                        
    bp_16 rsvd1:1;                      
    bp_16 rsvd2:1;                      
    bp_16 rsvd3:5;                      
    bp_32 rsvd4:16;                     
};

union CAN_ES_REG {
    bp_32  all;
    struct  CAN_ES_BITS  bit;
};

struct CAN_ERRC_BITS {                  
    bp_16 TEC:8;                        
    bp_16 REC:7;                        
    bp_16 RP:1;                         
    bp_32 rsvd1:16;                     
};

union CAN_ERRC_REG {
    bp_32  all;
    struct  CAN_ERRC_BITS  bit;
};

struct CAN_BTR_BITS {                   
    bp_16 BRP:6;                        
    bp_16 SJW:2;                        
    bp_16 TSEG1:4;                      
    bp_16 TSEG2:3;                      
    bp_16 rsvd1:1;                      
    bp_32 BRPE:4;                       
    bp_32 rsvd2:12;                     
};

union CAN_BTR_REG {
    bp_32  all;
    struct  CAN_BTR_BITS  bit;
};

struct CAN_INT_BITS {                   
    bp_16 INT0ID:16;                    
    bp_32 INT1ID:8;                     
    bp_32 rsvd1:8;                      
};

union CAN_INT_REG {
    bp_32  all;
    struct  CAN_INT_BITS  bit;
};

struct CAN_TEST_BITS {                  
    bp_16 rsvd1:3;                      
    bp_16 SILENT:1;                     
    bp_16 LBACK:1;                      
    bp_16 TX:2;                         
    bp_16 RX:1;                         
    bp_16 EXL:1;                        
    bp_16 RDA:1;                        
    bp_16 rsvd2:6;                      
    bp_32 rsvd3:16;                     
};

union CAN_TEST_REG {
    bp_32  all;
    struct  CAN_TEST_BITS  bit;
};

struct CAN_PERR_BITS {                  
    bp_16 MSG_NUM:8;                    
    bp_16 WORD_NUM:3;                   
    bp_16 rsvd1:5;                      
    bp_32 rsvd2:16;                     
};

union CAN_PERR_REG {
    bp_32  all;
    struct  CAN_PERR_BITS  bit;
};

struct CAN_RAM_INIT_BITS {              
    bp_16 KEY0:1;                       
    bp_16 KEY1:1;                       
    bp_16 KEY2:1;                       
    bp_16 KEY3:1;                       
    bp_16 CAN_RAM_INIT:1;               
    bp_16 RAM_INIT_DONE:1;              
    bp_16 rsvd1:10;                     
    bp_32 rsvd2:16;                     
};

union CAN_RAM_INIT_REG {
    bp_32  all;
    struct  CAN_RAM_INIT_BITS  bit;
};

struct CAN_GLB_INT_EN_BITS {            
    bp_16 GLBINT0_EN:1;                 
    bp_16 GLBINT1_EN:1;                 
    bp_16 rsvd1:14;                     
    bp_32 rsvd2:16;                     
};

union CAN_GLB_INT_EN_REG {
    bp_32  all;
    struct  CAN_GLB_INT_EN_BITS  bit;
};

struct CAN_GLB_INT_FLG_BITS {           
    bp_16 INT0_FLG:1;                   
    bp_16 INT1_FLG:1;                   
    bp_16 rsvd1:14;                     
    bp_32 rsvd2:16;                     
};

union CAN_GLB_INT_FLG_REG {
    bp_32  all;
    struct  CAN_GLB_INT_FLG_BITS  bit;
};

struct CAN_GLB_INT_CLR_BITS {           
    bp_16 INT0_FLG_CLR:1;               
    bp_16 INT1_FLG_CLR:1;               
    bp_16 rsvd1:14;                     
    bp_32 rsvd2:16;                     
};

union CAN_GLB_INT_CLR_REG {
    bp_32  all;
    struct  CAN_GLB_INT_CLR_BITS  bit;
};

struct CAN_TXRQ_X_BITS {                
    bp_16 TxRqstReg1:2;                 
    bp_16 TxRqstReg2:2;                 
    bp_16 rsvd1:12;                     
    bp_32 rsvd2:16;                     
};

union CAN_TXRQ_X_REG {
    bp_32  all;
    struct  CAN_TXRQ_X_BITS  bit;
};

struct CAN_NDAT_X_BITS {                
    bp_16 NewDatReg1:2;                 
    bp_16 NewDatReg2:2;                 
    bp_16 rsvd1:12;                     
    bp_32 rsvd2:16;                     
};

union CAN_NDAT_X_REG {
    bp_32  all;
    struct  CAN_NDAT_X_BITS  bit;
};

struct CAN_IPEN_X_BITS {                
    bp_16 IntPndReg1:2;                 
    bp_16 IntPndReg2:2;                 
    bp_16 rsvd1:12;                     
    bp_32 rsvd2:16;                     
};

union CAN_IPEN_X_REG {
    bp_32  all;
    struct  CAN_IPEN_X_BITS  bit;
};

struct CAN_MVAL_X_BITS {                
    bp_16 MsgValReg1:2;                 
    bp_16 MsgValReg2:2;                 
    bp_16 rsvd1:12;                     
    bp_32 rsvd2:16;                     
};

union CAN_MVAL_X_REG {
    bp_32  all;
    struct  CAN_MVAL_X_BITS  bit;
};

struct CAN_IF1CMD_BITS {                
    bp_16 MSG_NUM:8;                    
    bp_16 rsvd1:6;                      
    bp_16 DMAactive:1;                  
    bp_16 Busy:1;                       
    bp_32 DATA_B:1;                     
    bp_32 DATA_A:1;                     
    bp_32 TXRQST:1;                     
    bp_32 ClrIntPnd:1;                  
    bp_32 Control:1;                    
    bp_32 Arb:1;                        
    bp_32 Mask:1;                       
    bp_32 DIR:1;                        
    bp_32 rsvd2:8;                      
};

union CAN_IF1CMD_REG {
    bp_32  all;
    struct  CAN_IF1CMD_BITS  bit;
};

struct CAN_IF1MSK_BITS {                
    bp_32 Msk:29;                       
    bp_32 rsvd1:1;                      
    bp_32 MDir:1;                       
    bp_32 MXtd:1;                       
};

union CAN_IF1MSK_REG {
    bp_32  all;
    struct  CAN_IF1MSK_BITS  bit;
};

struct CAN_IF1ARB_BITS {                
    bp_32 ID:29;                        
    bp_32 Dir:1;                        
    bp_32 Xtd:1;                        
    bp_32 MsgVal:1;                     
};

union CAN_IF1ARB_REG {
    bp_32  all;
    struct  CAN_IF1ARB_BITS  bit;
};

struct CAN_IF1MCTL_BITS {               
    bp_16 DLC:4;                        
    bp_16 rsvd1:3;                      
    bp_16 EoB:1;                        
    bp_16 TxRqst:1;                     
    bp_16 RmtEn:1;                      
    bp_16 RxIE:1;                       
    bp_16 TxIE:1;                       
    bp_16 UMask:1;                      
    bp_16 IntPnd:1;                     
    bp_16 MsgLst:1;                     
    bp_16 NewDat:1;                     
    bp_32 rsvd2:16;                     
};

union CAN_IF1MCTL_REG {
    bp_32  all;
    struct  CAN_IF1MCTL_BITS  bit;
};

struct CAN_IF1DATA_BITS {               
    bp_16 Data_0:8;                     
    bp_16 Data_1:8;                     
    bp_32 Data_2:8;                     
    bp_32 Data_3:8;                     
};

union CAN_IF1DATA_REG {
    bp_32  all;
    struct  CAN_IF1DATA_BITS  bit;
};

struct CAN_IF1DATB_BITS {               
    bp_16 Data_4:8;                     
    bp_16 Data_5:8;                     
    bp_32 Data_6:8;                     
    bp_32 Data_7:8;                     
};

union CAN_IF1DATB_REG {
    bp_32  all;
    struct  CAN_IF1DATB_BITS  bit;
};

struct CAN_IF2CMD_BITS {                
    bp_16 MSG_NUM:8;                    
    bp_16 rsvd1:6;                      
    bp_16 DMAactive:1;                  
    bp_16 Busy:1;                       
    bp_32 DATA_B:1;                     
    bp_32 DATA_A:1;                     
    bp_32 TxRqst:1;                     
    bp_32 ClrIntPnd:1;                  
    bp_32 Control:1;                    
    bp_32 Arb:1;                        
    bp_32 Mask:1;                       
    bp_32 DIR:1;                        
    bp_32 rsvd2:8;                      
};

union CAN_IF2CMD_REG {
    bp_32  all;
    struct  CAN_IF2CMD_BITS  bit;
};

struct CAN_IF2MSK_BITS {                
    bp_32 Msk:29;                       
    bp_32 rsvd1:1;                      
    bp_32 MDir:1;                       
    bp_32 MXtd:1;                       
};

union CAN_IF2MSK_REG {
    bp_32  all;
    struct  CAN_IF2MSK_BITS  bit;
};

struct CAN_IF2ARB_BITS {                
    bp_32 ID:29;                        
    bp_32 Dir:1;                        
    bp_32 Xtd:1;                        
    bp_32 MsgVal:1;                     
};

union CAN_IF2ARB_REG {
    bp_32  all;
    struct  CAN_IF2ARB_BITS  bit;
};

struct CAN_IF2MCTL_BITS {               
    bp_16 DLC:4;                        
    bp_16 rsvd1:3;                      
    bp_16 EoB:1;                        
    bp_16 TxRqst:1;                     
    bp_16 RmtEn:1;                      
    bp_16 RxIE:1;                       
    bp_16 TxIE:1;                       
    bp_16 UMask:1;                      
    bp_16 IntPnd:1;                     
    bp_16 MsgLst:1;                     
    bp_16 NewDat:1;                     
    bp_32 rsvd2:16;                     
};

union CAN_IF2MCTL_REG {
    bp_32  all;
    struct  CAN_IF2MCTL_BITS  bit;
};

struct CAN_IF2DATA_BITS {               
    bp_16 Data_0:8;                     
    bp_16 Data_1:8;                     
    bp_32 Data_2:8;                     
    bp_32 Data_3:8;                     
};

union CAN_IF2DATA_REG {
    bp_32  all;
    struct  CAN_IF2DATA_BITS  bit;
};

struct CAN_IF2DATB_BITS {               
    bp_16 Data_4:8;                     
    bp_16 Data_5:8;                     
    bp_32 Data_6:8;                     
    bp_32 Data_7:8;                     
};

union CAN_IF2DATB_REG {
    bp_32  all;
    struct  CAN_IF2DATB_BITS  bit;
};

struct CAN_IF3OBS_BITS {                
    bp_16 Mask:1;                       
    bp_16 Arb:1;                        
    bp_16 Ctrl:1;                       
    bp_16 Data_A:1;                     
    bp_16 Data_B:1;                     
    bp_16 rsvd1:3;                      
    bp_16 IF3SM:1;                      
    bp_16 IF3SA:1;                      
    bp_16 IF3SC:1;                      
    bp_16 IF3SDA:1;                     
    bp_16 IF3SDB:1;                     
    bp_16 rsvd2:2;                      
    bp_16 IF3Upd:1;                     
    bp_32 rsvd3:16;                     
};

union CAN_IF3OBS_REG {
    bp_32  all;
    struct  CAN_IF3OBS_BITS  bit;
};

struct CAN_IF3MSK_BITS {                
    bp_32 Msk:29;                       
    bp_32 rsvd1:1;                      
    bp_32 MDir:1;                       
    bp_32 MXtd:1;                       
};

union CAN_IF3MSK_REG {
    bp_32  all;
    struct  CAN_IF3MSK_BITS  bit;
};

struct CAN_IF3ARB_BITS {                
    bp_32 ID:29;                        
    bp_32 Dir:1;                        
    bp_32 Xtd:1;                        
    bp_32 MsgVal:1;                     
};

union CAN_IF3ARB_REG {
    bp_32  all;
    struct  CAN_IF3ARB_BITS  bit;
};

struct CAN_IF3MCTL_BITS {               
    bp_16 DLC:4;                        
    bp_16 rsvd1:3;                      
    bp_16 EoB:1;                        
    bp_16 TxRqst:1;                     
    bp_16 RmtEn:1;                      
    bp_16 RxIE:1;                       
    bp_16 TxIE:1;                       
    bp_16 UMask:1;                      
    bp_16 IntPnd:1;                     
    bp_16 MsgLst:1;                     
    bp_16 NewDat:1;                     
    bp_32 rsvd2:16;                     
};

union CAN_IF3MCTL_REG {
    bp_32  all;
    struct  CAN_IF3MCTL_BITS  bit;
};

struct CAN_IF3DATA_BITS {               
    bp_16 Data_0:8;                     
    bp_16 Data_1:8;                     
    bp_32 Data_2:8;                     
    bp_32 Data_3:8;                     
};

union CAN_IF3DATA_REG {
    bp_32  all;
    struct  CAN_IF3DATA_BITS  bit;
};

struct CAN_IF3DATB_BITS {               
    bp_16 Data_4:8;                     
    bp_16 Data_5:8;                     
    bp_32 Data_6:8;                     
    bp_32 Data_7:8;                     
};

union CAN_IF3DATB_REG {
    bp_32  all;
    struct  CAN_IF3DATB_BITS  bit;
};

struct CAN_REGS {
    union   CAN_CTL_REG                      CAN_CTL;                      
    union   CAN_ES_REG                       CAN_ES;                       
    union   CAN_ERRC_REG                     CAN_ERRC;                     
    union   CAN_BTR_REG                      CAN_BTR;                      
    union   CAN_INT_REG                      CAN_INT;                      
    union   CAN_TEST_REG                     CAN_TEST;                     
    uint32_t                                 rsvd1[2];                     
    union   CAN_PERR_REG                     CAN_PERR;                     
    uint32_t                                 rsvd2[16];                    
    union   CAN_RAM_INIT_REG                 CAN_RAM_INIT;                 
    uint32_t                                 rsvd3[6];                     
    union   CAN_GLB_INT_EN_REG               CAN_GLB_INT_EN;               
    union   CAN_GLB_INT_FLG_REG              CAN_GLB_INT_FLG;              
    union   CAN_GLB_INT_CLR_REG              CAN_GLB_INT_CLR;              
    uint32_t                                 rsvd4[18];                    
    bp_32                                    CAN_ABOTR;                    
    union   CAN_TXRQ_X_REG                   CAN_TXRQ_X;                   
    bp_32                                    CAN_TXRQ_21;                  
    uint32_t                                 rsvd5[6];                     
    union   CAN_NDAT_X_REG                   CAN_NDAT_X;                   
    bp_32                                    CAN_NDAT_21;                  
    uint32_t                                 rsvd6[6];                     
    union   CAN_IPEN_X_REG                   CAN_IPEN_X;                   
    bp_32                                    CAN_IPEN_21;                  
    uint32_t                                 rsvd7[6];                     
    union   CAN_MVAL_X_REG                   CAN_MVAL_X;                   
    bp_32                                    CAN_MVAL_21;                  
    uint32_t                                 rsvd8[8];                     
    bp_32                                    CAN_IP_MUX21;                 
    uint32_t                                 rsvd9[18];                    
    union   CAN_IF1CMD_REG                   CAN_IF1CMD;                   
    union   CAN_IF1MSK_REG                   CAN_IF1MSK;                   
    union   CAN_IF1ARB_REG                   CAN_IF1ARB;                   
    union   CAN_IF1MCTL_REG                  CAN_IF1MCTL;                  
    union   CAN_IF1DATA_REG                  CAN_IF1DATA;                  
    union   CAN_IF1DATB_REG                  CAN_IF1DATB;                  
    uint32_t                                 rsvd10[4];                    
    union   CAN_IF2CMD_REG                   CAN_IF2CMD;                   
    union   CAN_IF2MSK_REG                   CAN_IF2MSK;                   
    union   CAN_IF2ARB_REG                   CAN_IF2ARB;                   
    union   CAN_IF2MCTL_REG                  CAN_IF2MCTL;                  
    union   CAN_IF2DATA_REG                  CAN_IF2DATA;                  
    union   CAN_IF2DATB_REG                  CAN_IF2DATB;                  
    uint32_t                                 rsvd11[4];                    
    union   CAN_IF3OBS_REG                   CAN_IF3OBS;                   
    union   CAN_IF3MSK_REG                   CAN_IF3MSK;                   
    union   CAN_IF3ARB_REG                   CAN_IF3ARB;                   
    union   CAN_IF3MCTL_REG                  CAN_IF3MCTL;                  
    union   CAN_IF3DATA_REG                  CAN_IF3DATA;                  
    union   CAN_IF3DATB_REG                  CAN_IF3DATB;                  
    uint32_t                                 rsvd12[4];                    
    bp_32                                    CAN_IF3UPD;                   
};




extern volatile struct CAN_REGS CanaRegs;



















































struct DCCGCTRL_BITS {                  
    bp_16 DCCENA:4;                     
    bp_16 ERRENA:4;                     
    bp_16 SINGLESHOT:4;                 
    bp_16 DONEENA:4;                    
    bp_32 rsvd1:16;                     
};

union DCCGCTRL_REG {
    bp_32  all;
    struct  DCCGCTRL_BITS  bit;
};

struct DCCCNTSEED0_BITS {               
    bp_32 COUNTSEED0:20;                
    bp_32 rsvd1:12;                     
};

union DCCCNTSEED0_REG {
    bp_32  all;
    struct  DCCCNTSEED0_BITS  bit;
};

struct DCCVALIDSEED0_BITS {             
    bp_16 VALIDSEED:16;                 
    bp_32 rsvd1:16;                     
};

union DCCVALIDSEED0_REG {
    bp_32  all;
    struct  DCCVALIDSEED0_BITS  bit;
};

struct DCCCNTSEED1_BITS {               
    bp_32 COUNTSEED1:20;                
    bp_32 rsvd1:12;                     
};

union DCCCNTSEED1_REG {
    bp_32  all;
    struct  DCCCNTSEED1_BITS  bit;
};

struct DCCSTATUS_BITS {                 
    bp_16 ERR:1;                        
    bp_16 DONE:1;                       
    bp_16 rsvd1:14;                     
    bp_32 rsvd2:16;                     
};

union DCCSTATUS_REG {
    bp_32  all;
    struct  DCCSTATUS_BITS  bit;
};

struct DCCCNT0_BITS {                   
    bp_32 COUNT0:20;                    
    bp_32 rsvd1:12;                     
};

union DCCCNT0_REG {
    bp_32  all;
    struct  DCCCNT0_BITS  bit;
};

struct DCCVALID0_BITS {                 
    bp_16 VALID0:16;                    
    bp_32 rsvd1:16;                     
};

union DCCVALID0_REG {
    bp_32  all;
    struct  DCCVALID0_BITS  bit;
};

struct DCCCNT1_BITS {                   
    bp_32 COUNT1:20;                    
    bp_32 rsvd1:12;                     
};

union DCCCNT1_REG {
    bp_32  all;
    struct  DCCCNT1_BITS  bit;
};

struct DCCCLKSRC1_BITS {                
    bp_16 CLKSRC1:5;                    
    bp_16 rsvd1:7;                      
    bp_16 KEY:4;                        
    bp_32 rsvd2:16;                     
};

union DCCCLKSRC1_REG {
    bp_32  all;
    struct  DCCCLKSRC1_BITS  bit;
};

struct DCCCLKSRC0_BITS {                
    bp_16 CLKSRC0:4;                    
    bp_16 rsvd1:8;                      
    bp_16 KEY:4;                        
    bp_32 rsvd2:16;                     
};

union DCCCLKSRC0_REG {
    bp_32  all;
    struct  DCCCLKSRC0_BITS  bit;
};

struct DCC_REGS {
    union   DCCGCTRL_REG                     DCCGCTRL;                     
    uint32_t                                 rsvd1[2];                     
    union   DCCCNTSEED0_REG                  DCCCNTSEED0;                  
    union   DCCVALIDSEED0_REG                DCCVALIDSEED0;                
    union   DCCCNTSEED1_REG                  DCCCNTSEED1;                  
    union   DCCSTATUS_REG                    DCCSTATUS;                    
    union   DCCCNT0_REG                      DCCCNT0;                      
    union   DCCVALID0_REG                    DCCVALID0;                    
    union   DCCCNT1_REG                      DCCCNT1;                      
    union   DCCCLKSRC1_REG                   DCCCLKSRC1;                   
    union   DCCCLKSRC0_REG                   DCCCLKSRC0;                   
};




extern volatile struct DCC_REGS Dcc0Regs;
extern volatile struct DCC_REGS Dcc1Regs;



















































struct SCIGCR0_BITS {                   
    bp_16 RESET:1;                      
    bp_16 rsvd1:15;                     
    bp_32 rsvd2:16;                     
};

union SCIGCR0_REG {
    bp_32  all;
    struct  SCIGCR0_BITS  bit;
};

struct SCIGCR1_BITS {                   
    bp_16 COMMMODE:1;                   
    bp_16 TIMINGMODE:1;                 
    bp_16 PARITYENA:1;                  
    bp_16 PARITY:1;                     
    bp_16 STOP:1;                       
    bp_16 CLK_MASTER:1;                 
    bp_16 LINMODE:1;                    
    bp_16 SWnRST:1;                     
    bp_16 SLEEP:1;                      
    bp_16 ADAPT:1;                      
    bp_16 MBUFMODE:1;                   
    bp_16 CTYPE:1;                      
    bp_16 HGENCTRL:1;                   
    bp_16 STOPEXTFRAME:1;               
    bp_16 rsvd1:2;                      
    bp_32 LOOPBACK:1;                   
    bp_32 CONT:1;                       
    bp_32 rsvd2:6;                      
    bp_32 RXENA:1;                      
    bp_32 TXENA:1;                      
    bp_32 rsvd3:6;                      
};

union SCIGCR1_REG {
    bp_32  all;
    struct  SCIGCR1_BITS  bit;
};

struct SCIGCR2_BITS {                   
    bp_16 POWERDOWN:1;                  
    bp_16 rsvd1:7;                      
    bp_16 GENWU:1;                      
    bp_16 rsvd2:7;                      
    bp_32 SC:1;                         
    bp_32 CC:1;                         
    bp_32 rsvd3:14;                     
};

union SCIGCR2_REG {
    bp_32  all;
    struct  SCIGCR2_BITS  bit;
};

struct SCISETINT_BITS {                 
    bp_16 SETBRKDTINT:1;                
    bp_16 SETWAKEUPINT:1;               
    bp_16 rsvd1:2;                      
    bp_16 SETTIMEOUTINT:1;              
    bp_16 rsvd2:1;                      
    bp_16 SETTOAWUSINT:1;               
    bp_16 SETTOA3WUSINT:1;              
    bp_16 SETTXINT:1;                   
    bp_16 SETRXINT:1;                   
    bp_16 rsvd3:3;                      
    bp_16 SETIDINT:1;                   
    bp_16 rsvd4:2;                      
    bp_32 SET_TX_DMA:1;                 
    bp_32 SET_RX_DMA:1;                 
    bp_32 SET_RX_DMA_ALL:1;             
    bp_32 rsvd5:5;                      
    bp_32 SETPEINT:1;                   
    bp_32 SETOEINT:1;                   
    bp_32 SETFEINT:1;                   
    bp_32 SETNREINT:1;                  
    bp_32 SETISFEINT:1;                 
    bp_32 SETCEINT:1;                   
    bp_32 SETPBEINT:1;                  
    bp_32 SETBEINT:1;                   
};

union SCISETINT_REG {
    bp_32  all;
    struct  SCISETINT_BITS  bit;
};

struct SCICLEARINT_BITS {               
    bp_16 CLRBRKDTINT:1;                
    bp_16 CLRWAKEUPINT:1;               
    bp_16 rsvd1:2;                      
    bp_16 CLRTIMEOUTINT:1;              
    bp_16 rsvd2:1;                      
    bp_16 CLRTOAWUSINT:1;               
    bp_16 CLRTOA3WUSINT:1;              
    bp_16 CLRTXINT:1;                   
    bp_16 CLRRXINT:1;                   
    bp_16 rsvd3:3;                      
    bp_16 CLRIDINT:1;                   
    bp_16 rsvd4:2;                      
    bp_32 CLRTXDMA:1;                   
    bp_32 SETRXDMA:1;                   
    bp_32 rsvd5:1;                      
    bp_32 rsvd6:5;                      
    bp_32 CLRPEINT:1;                   
    bp_32 CLROEINT:1;                   
    bp_32 CLRFEINT:1;                   
    bp_32 CLRNREINT:1;                  
    bp_32 CLRISFEINT:1;                 
    bp_32 CLRCEINT:1;                   
    bp_32 CLRPBEINT:1;                  
    bp_32 CLRBEINT:1;                   
};

union SCICLEARINT_REG {
    bp_32  all;
    struct  SCICLEARINT_BITS  bit;
};

struct SCISETINTLVL_BITS {              
    bp_16 SETBRKDTINTLVL:1;             
    bp_16 SETWAKEUPINTLVL:1;            
    bp_16 rsvd1:2;                      
    bp_16 SETTIMEOUTINTLVL:1;           
    bp_16 rsvd2:1;                      
    bp_16 SETTOAWUSINTLVL:1;            
    bp_16 SETTOA3WUSINTLVL:1;           
    bp_16 SETTXINTLVL:1;                
    bp_16 SETRXINTOVO:1;                
    bp_16 rsvd3:3;                      
    bp_16 SETIDINTLVL:1;                
    bp_16 rsvd4:2;                      
    bp_32 rsvd5:2;                      
    bp_32 rsvd6:1;                      
    bp_32 rsvd7:5;                      
    bp_32 SETPEINTLVL:1;                
    bp_32 SETOEINTLVL:1;                
    bp_32 SETFEINTLVL:1;                
    bp_32 SETNREINTLVL:1;               
    bp_32 SETISFEINTLVL:1;              
    bp_32 SETCEINTLVL:1;                
    bp_32 SETPBEINTLVL:1;               
    bp_32 SETBEINTLVL:1;                
};

union SCISETINTLVL_REG {
    bp_32  all;
    struct  SCISETINTLVL_BITS  bit;
};

struct SCICLEARINTLVL_BITS {            
    bp_16 CLRBRKDTINTLVL:1;             
    bp_16 CLRWAKEUPINTLVL:1;            
    bp_16 rsvd1:2;                      
    bp_16 CLRTIMEOUTINTLVL:1;           
    bp_16 rsvd2:1;                      
    bp_16 CLRTOAWUSINTLVL:1;            
    bp_16 CLRTOA3WUSINTLVL:1;           
    bp_16 CLRTXINTLVL:1;                
    bp_16 CLRRXINTLVL:1;                
    bp_16 rsvd3:3;                      
    bp_16 CLRIDINTLVL:1;                
    bp_16 rsvd4:2;                      
    bp_32 rsvd5:2;                      
    bp_32 rsvd6:1;                      
    bp_32 rsvd7:5;                      
    bp_32 CLRPEINTLVL:1;                
    bp_32 CLROEINTLVL:1;                
    bp_32 CLRFEINTLVL:1;                
    bp_32 CLRNREINTLVL:1;               
    bp_32 CLRISFEINTLVL:1;              
    bp_32 CLRCEINTLVL:1;                
    bp_32 CLRPBEINTLVL:1;               
    bp_32 CLRBEINTLVL:1;                
};

union SCICLEARINTLVL_REG {
    bp_32  all;
    struct  SCICLEARINTLVL_BITS  bit;
};

struct SCIFLR_BITS {                    
    bp_16 BRKDT:1;                      
    bp_16 WAKEUP:1;                     
    bp_16 IDLE_:1;                       
    bp_16 BUSY:1;                       
    bp_16 TIMEOUT:1;                    
    bp_16 rsvd1:1;                      
    bp_16 TOAWUS:1;                     
    bp_16 TOA3WUS:1;                    
    bp_16 TXRDY:1;                      
    bp_16 RXRDY:1;                      
    bp_16 TXWAKE:1;                     
    bp_16 TXEMPTY:1;                    
    bp_16 RXWAKE:1;                     
    bp_16 IDTXFLAG:1;                   
    bp_16 IDRXFLAG:1;                   
    bp_16 rsvd2:1;                      
    bp_32 rsvd3:8;                      
    bp_32 PE:1;                         
    bp_32 OE:1;                         
    bp_32 FE:1;                         
    bp_32 NRE:1;                        
    bp_32 ISFE:1;                       
    bp_32 CE:1;                         
    bp_32 PBE:1;                        
    bp_32 BE:1;                         
};

union SCIFLR_REG {
    bp_32  all;
    struct  SCIFLR_BITS  bit;
};

struct SCIINTVECT0_BITS {               
    bp_16 INTVECT0:5;                   
    bp_16 rsvd1:11;                     
    bp_32 rsvd2:16;                     
};

union SCIINTVECT0_REG {
    bp_32  all;
    struct  SCIINTVECT0_BITS  bit;
};

struct SCIINTVECT1_BITS {               
    bp_16 INTVECT1:5;                   
    bp_16 rsvd1:11;                     
    bp_32 rsvd2:16;                     
};

union SCIINTVECT1_REG {
    bp_32  all;
    struct  SCIINTVECT1_BITS  bit;
};

struct SCIFORMAT_BITS {                 
    bp_16 CHAR:3;                       
    bp_16 rsvd1:13;                     
    bp_32 LENGTH:3;                     
    bp_32 rsvd2:13;                     
};

union SCIFORMAT_REG {
    bp_32  all;
    struct  SCIFORMAT_BITS  bit;
};

struct BRSR_BITS {                      
    bp_16 SCI_LIN_PSL:16;               
    bp_32 SCI_LIN_PSH:8;                
    bp_32 M:4;                          
    bp_32 U:3;                          
    bp_32 rsvd1:1;                      
};

union BRSR_REG {
    bp_32  all;
    struct  BRSR_BITS  bit;
};

struct SCIED_BITS {                     
    bp_16 ED:8;                         
    bp_16 rsvd1:8;                      
    bp_32 rsvd2:16;                     
};

union SCIED_REG {
    bp_32  all;
    struct  SCIED_BITS  bit;
};

struct SCIRD_BITS {                     
    bp_16 RD:8;                         
    bp_16 rsvd1:8;                      
    bp_32 rsvd2:16;                     
};

union SCIRD_REG {
    bp_32  all;
    struct  SCIRD_BITS  bit;
};

struct SCITD_BITS {                     
    bp_16 TD:8;                         
    bp_16 rsvd1:8;                      
    bp_32 rsvd2:16;                     
};

union SCITD_REG {
    bp_32  all;
    struct  SCITD_BITS  bit;
};

struct SCIPIO0_BITS {                   
    bp_16 rsvd1:1;                      
    bp_16 RXFUNC:1;                     
    bp_16 TXFUNC:1;                     
    bp_16 rsvd2:13;                     
    bp_32 rsvd3:16;                     
};

union SCIPIO0_REG {
    bp_32  all;
    struct  SCIPIO0_BITS  bit;
};

struct SCIPIO2_BITS {                   
    bp_16 rsvd1:1;                      
    bp_16 RXIN:1;                       
    bp_16 TXIN:1;                       
    bp_16 rsvd2:13;                     
    bp_32 rsvd3:16;                     
};

union SCIPIO2_REG {
    bp_32  all;
    struct  SCIPIO2_BITS  bit;
};

struct LINCOMP_BITS {                   
    bp_16 SBREAK:3;                     
    bp_16 rsvd1:5;                      
    bp_16 SDEL:2;                       
    bp_16 rsvd2:6;                      
    bp_32 rsvd3:16;                     
};

union LINCOMP_REG {
    bp_32  all;
    struct  LINCOMP_BITS  bit;
};

struct LINRD0_BITS {                    
    bp_16 RD3:8;                        
    bp_16 RD2:8;                        
    bp_32 RD1:8;                        
    bp_32 RD0:8;                        
};

union LINRD0_REG {
    bp_32  all;
    struct  LINRD0_BITS  bit;
};

struct LINRD1_BITS {                    
    bp_16 RD7:8;                        
    bp_16 RD6:8;                        
    bp_32 RD5:8;                        
    bp_32 RD4:8;                        
};

union LINRD1_REG {
    bp_32  all;
    struct  LINRD1_BITS  bit;
};

struct LINMASK_BITS {                   
    bp_16 TXIDMASK:8;                   
    bp_16 rsvd1:8;                      
    bp_32 RXIDMASK:8;                   
    bp_32 rsvd2:8;                      
};

union LINMASK_REG {
    bp_32  all;
    struct  LINMASK_BITS  bit;
};

struct LINID_BITS {                     
    bp_16 IDBYTE:8;                     
    bp_16 IDSLAVETASKBYTE:8;            
    bp_32 RECEIVEDID:8;                 
    bp_32 rsvd1:8;                      
};

union LINID_REG {
    bp_32  all;
    struct  LINID_BITS  bit;
};

struct LINTD0_BITS {                    
    bp_16 TD3:8;                        
    bp_16 TD2:8;                        
    bp_32 TD1:8;                        
    bp_32 TD0:8;                        
};

union LINTD0_REG {
    bp_32  all;
    struct  LINTD0_BITS  bit;
};

struct LINTD1_BITS {                    
    bp_16 TD7:8;                        
    bp_16 TD6:8;                        
    bp_32 TD5:8;                        
    bp_32 TD4:8;                        
};

union LINTD1_REG {
    bp_32  all;
    struct  LINTD1_BITS  bit;
};

struct MBRSR_BITS {                     
    bp_16 MBR:13;                       
    bp_16 rsvd1:3;                      
    bp_32 rsvd2:16;                     
};

union MBRSR_REG {
    bp_32  all;
    struct  MBRSR_BITS  bit;
};

struct IODFTCTRL_BITS {                 
    bp_16 RXPENA:1;                     
    bp_16 LPBENA:1;                     
    bp_16 rsvd1:6;                      
    bp_16 IODFTENA:4;                   
    bp_16 rsvd2:4;                      
    bp_32 TXSHIFT:3;                    
    bp_32 PINSAMPLEMASK:2;              
    bp_32 rsvd3:3;                      
    bp_32 BRKDTERRENA:1;                
    bp_32 PERRENA:1;                    
    bp_32 FERRENA:1;                    
    bp_32 rsvd4:1;                      
    bp_32 ISFERRENA:1;                  
    bp_32 CERRENA:1;                    
    bp_32 PBERRENA:1;                   
    bp_32 BERRENA:1;                    
};

union IODFTCTRL_REG {
    bp_32  all;
    struct  IODFTCTRL_BITS  bit;
};

struct LIN_GLB_INT_EN_BITS {            
    bp_16 GLBINT0_EN:1;                 
    bp_16 GLBINT1_EN:1;                 
    bp_16 rsvd1:14;                     
    bp_32 rsvd2:16;                     
};

union LIN_GLB_INT_EN_REG {
    bp_32  all;
    struct  LIN_GLB_INT_EN_BITS  bit;
};

struct LIN_GLB_INT_FLG_BITS {           
    bp_16 INT0_FLG:1;                   
    bp_16 INT1_FLG:1;                   
    bp_16 rsvd1:14;                     
    bp_32 rsvd2:16;                     
};

union LIN_GLB_INT_FLG_REG {
    bp_32  all;
    struct  LIN_GLB_INT_FLG_BITS  bit;
};

struct LIN_GLB_INT_CLR_BITS {           
    bp_16 INT0_FLG_CLR:1;               
    bp_16 INT1_FLG_CLR:1;               
    bp_16 rsvd1:14;                     
    bp_32 rsvd2:16;                     
};

union LIN_GLB_INT_CLR_REG {
    bp_32  all;
    struct  LIN_GLB_INT_CLR_BITS  bit;
};

struct LIN_REGS {
    union   SCIGCR0_REG                      SCIGCR0;                      
    union   SCIGCR1_REG                      SCIGCR1;                      
    union   SCIGCR2_REG                      SCIGCR2;                      
    union   SCISETINT_REG                    SCISETINT;                    
    union   SCICLEARINT_REG                  SCICLEARINT;                  
    union   SCISETINTLVL_REG                 SCISETINTLVL;                 
    union   SCICLEARINTLVL_REG               SCICLEARINTLVL;               
    union   SCIFLR_REG                       SCIFLR;                       
    union   SCIINTVECT0_REG                  SCIINTVECT0;                  
    union   SCIINTVECT1_REG                  SCIINTVECT1;                  
    union   SCIFORMAT_REG                    SCIFORMAT;                    
    union   BRSR_REG                         BRSR;                         
    union   SCIED_REG                        SCIED;                        
    union   SCIRD_REG                        SCIRD;                        
    union   SCITD_REG                        SCITD;                        
    union   SCIPIO0_REG                      SCIPIO0;                      
    uint32_t                                 rsvd1[2];                     
    union   SCIPIO2_REG                      SCIPIO2;                      
    uint32_t                                 rsvd2[12];                    
    union   LINCOMP_REG                      LINCOMP;                      
    union   LINRD0_REG                       LINRD0;                       
    union   LINRD1_REG                       LINRD1;                       
    union   LINMASK_REG                      LINMASK;                      
    union   LINID_REG                        LINID;                        
    union   LINTD0_REG                       LINTD0;                       
    union   LINTD1_REG                       LINTD1;                       
    union   MBRSR_REG                        MBRSR;                        
    uint32_t                                 rsvd3[8];                     
    union   IODFTCTRL_REG                    IODFTCTRL;                    
    uint32_t                                 rsvd4[38];                    
    union   LIN_GLB_INT_EN_REG               LIN_GLB_INT_EN;               
    union   LIN_GLB_INT_FLG_REG              LIN_GLB_INT_FLG;              
    union   LIN_GLB_INT_CLR_REG              LIN_GLB_INT_CLR;              
};




extern volatile struct LIN_REGS LinaRegs;
extern volatile struct LIN_REGS LinbRegs;






















































struct MCANSS_PID_BITS {                
    bp_16 MINOR:6;                      
    bp_16 rsvd1:2;                      
    bp_16 MAJOR:3;                      
    bp_16 rsvd2:5;                      
    bp_32 MODULE_ID:12;                 
    bp_32 rsvd3:2;                      
    bp_32 SCHEME:2;                     
};

union MCANSS_PID_REG {
    bp_32  all;
    struct  MCANSS_PID_BITS  bit;
};

struct MCANSS_CTRL_BITS {               
    bp_16 rsvd1:3;                      
    bp_16 DBGSUSP_FREE:1;               
    bp_16 WAKEUPREQEN:1;                
    bp_16 AUTOWAKEUP:1;                 
    bp_16 EXT_TS_CNTR_EN:1;             
    bp_16 rsvd2:9;                      
    bp_32 rsvd3:16;                     
};

union MCANSS_CTRL_REG {
    bp_32  all;
    struct  MCANSS_CTRL_BITS  bit;
};

struct MCANSS_STAT_BITS {               
    bp_16 RESET:1;                      
    bp_16 MEM_INIT_DONE:1;              
    bp_16 ENABLE_FDOE:1;                
    bp_16 rsvd1:13;                     
    bp_32 rsvd2:16;                     
};

union MCANSS_STAT_REG {
    bp_32  all;
    struct  MCANSS_STAT_BITS  bit;
};

struct MCANSS_ICS_BITS {                
    bp_16 EXT_TS_CNTR_OVFL:1;           
    bp_16 rsvd1:15;                     
    bp_32 rsvd2:16;                     
};

union MCANSS_ICS_REG {
    bp_32  all;
    struct  MCANSS_ICS_BITS  bit;
};

struct MCANSS_IRS_BITS {                
    bp_16 EXT_TS_CNTR_OVFL:1;           
    bp_16 rsvd1:15;                     
    bp_32 rsvd2:16;                     
};

union MCANSS_IRS_REG {
    bp_32  all;
    struct  MCANSS_IRS_BITS  bit;
};

struct MCANSS_IECS_BITS {               
    bp_16 EXT_TS_CNTR_OVFL:1;           
    bp_16 rsvd1:15;                     
    bp_32 rsvd2:16;                     
};

union MCANSS_IECS_REG {
    bp_32  all;
    struct  MCANSS_IECS_BITS  bit;
};

struct MCANSS_IE_BITS {                 
    bp_16 EXT_TS_CNTR_OVFL:1;           
    bp_16 rsvd1:15;                     
    bp_32 rsvd2:16;                     
};

union MCANSS_IE_REG {
    bp_32  all;
    struct  MCANSS_IE_BITS  bit;
};

struct MCANSS_IES_BITS {                
    bp_16 EXT_TS_CNTR_OVFL:1;           
    bp_16 rsvd1:15;                     
    bp_32 rsvd2:16;                     
};

union MCANSS_IES_REG {
    bp_32  all;
    struct  MCANSS_IES_BITS  bit;
};

struct MCANSS_EOI_BITS {                
    bp_16 EOI:8;                        
    bp_16 rsvd1:8;                      
    bp_32 rsvd2:16;                     
};

union MCANSS_EOI_REG {
    bp_32  all;
    struct  MCANSS_EOI_BITS  bit;
};

struct MCANSS_EXT_TS_PRESCALER_BITS {   
    bp_32 PRESCALER:24;                 
    bp_32 rsvd1:8;                      
};

union MCANSS_EXT_TS_PRESCALER_REG {
    bp_32  all;
    struct  MCANSS_EXT_TS_PRESCALER_BITS  bit;
};

struct MCANSS_EXT_TS_UNSERVICED_INTR_CNTR_BITS {
    bp_16 EXT_TS_INTR_CNTR:5;           
    bp_16 rsvd1:11;                     
    bp_32 rsvd2:16;                     
};

union MCANSS_EXT_TS_UNSERVICED_INTR_CNTR_REG {
    bp_32  all;
    struct  MCANSS_EXT_TS_UNSERVICED_INTR_CNTR_BITS  bit;
};

struct MCANSS_REGS {
    union   MCANSS_PID_REG                   MCANSS_PID;                   
    union   MCANSS_CTRL_REG                  MCANSS_CTRL;                  
    union   MCANSS_STAT_REG                  MCANSS_STAT;                  
    union   MCANSS_ICS_REG                   MCANSS_ICS;                   
    union   MCANSS_IRS_REG                   MCANSS_IRS;                   
    union   MCANSS_IECS_REG                  MCANSS_IECS;                  
    union   MCANSS_IE_REG                    MCANSS_IE;                    
    union   MCANSS_IES_REG                   MCANSS_IES;                   
    union   MCANSS_EOI_REG                   MCANSS_EOI;                   
    union   MCANSS_EXT_TS_PRESCALER_REG      MCANSS_EXT_TS_PRESCALER;      
    union   MCANSS_EXT_TS_UNSERVICED_INTR_CNTR_REG MCANSS_EXT_TS_UNSERVICED_INTR_CNTR; 
};

struct MCAN_CREL_BITS {                 
    bp_16 DAY:8;                        
    bp_16 MON:8;                        
    bp_32 YEAR:4;                       
    bp_32 SUBSTEP:4;                    
    bp_32 STEP:4;                       
    bp_32 REL:4;                        
};

union MCAN_CREL_REG {
    bp_32  all;
    struct  MCAN_CREL_BITS  bit;
};

struct MCAN_DBTP_BITS {                 
    bp_16 DSJW:4;                       
    bp_16 DTSEG2:4;                     
    bp_16 DTSEG1:5;                     
    bp_16 rsvd1:3;                      
    bp_32 DBRP:5;                       
    bp_32 rsvd2:2;                      
    bp_32 TDC:1;                        
    bp_32 rsvd3:8;                      
};

union MCAN_DBTP_REG {
    bp_32  all;
    struct  MCAN_DBTP_BITS  bit;
};

struct MCAN_TEST_BITS {                 
    bp_16 rsvd1:4;                      
    bp_16 LBCK:1;                       
    bp_16 TX:2;                         
    bp_16 RX:1;                         
    bp_16 rsvd2:8;                      
    bp_32 rsvd3:16;                     
};

union MCAN_TEST_REG {
    bp_32  all;
    struct  MCAN_TEST_BITS  bit;
};

struct MCAN_RWD_BITS {                  
    bp_16 WDC:8;                        
    bp_16 WDV:8;                        
    bp_32 rsvd1:16;                     
};

union MCAN_RWD_REG {
    bp_32  all;
    struct  MCAN_RWD_BITS  bit;
};

struct MCAN_CCCR_BITS {                 
    bp_16 INIT:1;                       
    bp_16 CCE:1;                        
    bp_16 ASM:1;                        
    bp_16 CSA:1;                        
    bp_16 CSR:1;                        
    bp_16 MON:1;                        
    bp_16 DAR:1;                        
    bp_16 TEST:1;                       
    bp_16 FDOE:1;                       
    bp_16 BRSE:1;                       
    bp_16 rsvd1:2;                      
    bp_16 PXHD:1;                       
    bp_16 EFBI:1;                       
    bp_16 TXP:1;                        
    bp_16 NISO:1;                       
    bp_32 rsvd2:16;                     
};

union MCAN_CCCR_REG {
    bp_32  all;
    struct  MCAN_CCCR_BITS  bit;
};

struct MCAN_NBTP_BITS {                 
    bp_16 NTSEG2:7;                     
    bp_16 rsvd1:1;                      
    bp_16 NTSEG1:8;                     
    bp_32 NBRP:9;                       
    bp_32 NSJW:7;                       
};

union MCAN_NBTP_REG {
    bp_32  all;
    struct  MCAN_NBTP_BITS  bit;
};

struct MCAN_TSCC_BITS {                 
    bp_16 TSS:2;                        
    bp_16 rsvd1:14;                     
    bp_32 TCP:4;                        
    bp_32 rsvd2:12;                     
};

union MCAN_TSCC_REG {
    bp_32  all;
    struct  MCAN_TSCC_BITS  bit;
};

struct MCAN_TSCV_BITS {                 
    bp_16 TSC:16;                       
    bp_32 rsvd1:16;                     
};

union MCAN_TSCV_REG {
    bp_32  all;
    struct  MCAN_TSCV_BITS  bit;
};

struct MCAN_TOCC_BITS {                 
    bp_16 ETOC:1;                       
    bp_16 TOS:2;                        
    bp_16 rsvd1:13;                     
    bp_32 TOP:16;                       
};

union MCAN_TOCC_REG {
    bp_32  all;
    struct  MCAN_TOCC_BITS  bit;
};

struct MCAN_TOCV_BITS {                 
    bp_16 TOC:16;                       
    bp_32 rsvd1:16;                     
};

union MCAN_TOCV_REG {
    bp_32  all;
    struct  MCAN_TOCV_BITS  bit;
};

struct MCAN_ECR_BITS {                  
    bp_16 TEC:8;                        
    bp_16 REC:7;                        
    bp_16 RP:1;                         
    bp_32 CEL:8;                        
    bp_32 rsvd1:8;                      
};

union MCAN_ECR_REG {
    bp_32  all;
    struct  MCAN_ECR_BITS  bit;
};

struct MCAN_PSR_BITS {                  
    bp_16 LEC:3;                        
    bp_16 ACT:2;                        
    bp_16 EP:1;                         
    bp_16 EW:1;                         
    bp_16 BO:1;                         
    bp_16 DLEC:3;                       
    bp_16 RESI:1;                       
    bp_16 RBRS:1;                       
    bp_16 RFDF:1;                       
    bp_16 PXE:1;                        
    bp_16 rsvd1:1;                      
    bp_32 TDCV:7;                       
    bp_32 rsvd2:9;                      
};

union MCAN_PSR_REG {
    bp_32  all;
    struct  MCAN_PSR_BITS  bit;
};

struct MCAN_TDCR_BITS {                 
    bp_16 TDCF:7;                       
    bp_16 rsvd1:1;                      
    bp_16 TDCO:7;                       
    bp_16 rsvd2:1;                      
    bp_32 rsvd3:16;                     
};

union MCAN_TDCR_REG {
    bp_32  all;
    struct  MCAN_TDCR_BITS  bit;
};

struct MCAN_IR_BITS {                   
    bp_16 RF0N:1;                       
    bp_16 RF0W:1;                       
    bp_16 RF0F:1;                       
    bp_16 RF0L:1;                       
    bp_16 RF1N:1;                       
    bp_16 RF1W:1;                       
    bp_16 RF1F:1;                       
    bp_16 RF1L:1;                       
    bp_16 HPM:1;                        
    bp_16 TC:1;                         
    bp_16 TCF:1;                        
    bp_16 TFE:1;                        
    bp_16 TEFN:1;                       
    bp_16 TEFW:1;                       
    bp_16 TEFF:1;                       
    bp_16 TEFL:1;                       
    bp_32 TSW:1;                        
    bp_32 MRAF:1;                       
    bp_32 TOO:1;                        
    bp_32 DRX:1;                        
    bp_32 rsvd1:1;                      
    bp_32 BEU:1;                        
    bp_32 ELO:1;                        
    bp_32 EP:1;                         
    bp_32 EW:1;                         
    bp_32 BO:1;                         
    bp_32 WDI:1;                        
    bp_32 PEA:1;                        
    bp_32 PED:1;                        
    bp_32 ARA:1;                        
    bp_32 rsvd2:1;                      
    bp_32 rsvd3:1;                      
};

union MCAN_IR_REG {
    bp_32  all;
    struct  MCAN_IR_BITS  bit;
};

struct MCAN_IE_BITS {                   
    bp_16 RF0NE:1;                      
    bp_16 RF0WE:1;                      
    bp_16 RF0FE:1;                      
    bp_16 RF0LE:1;                      
    bp_16 RF1NE:1;                      
    bp_16 RF1WE:1;                      
    bp_16 RF1FE:1;                      
    bp_16 RF1LE:1;                      
    bp_16 HPME:1;                       
    bp_16 TCE:1;                        
    bp_16 TCFE:1;                       
    bp_16 TFEE:1;                       
    bp_16 TEFNE:1;                      
    bp_16 TEFWE:1;                      
    bp_16 TEFFE:1;                      
    bp_16 TEFLE:1;                      
    bp_32 TSWE:1;                       
    bp_32 MRAFE:1;                      
    bp_32 TOOE:1;                       
    bp_32 DRXE:1;                       
    bp_32 BECE:1;                       
    bp_32 BEUE:1;                       
    bp_32 ELOE:1;                       
    bp_32 EPE:1;                        
    bp_32 EWE:1;                        
    bp_32 BOE:1;                        
    bp_32 WDIE:1;                       
    bp_32 PEAE:1;                       
    bp_32 PEDE:1;                       
    bp_32 ARAE:1;                       
    bp_32 rsvd1:2;                      
};

union MCAN_IE_REG {
    bp_32  all;
    struct  MCAN_IE_BITS  bit;
};

struct MCAN_ILS_BITS {                  
    bp_16 RF0NL:1;                      
    bp_16 RF0WL:1;                      
    bp_16 RF0FL:1;                      
    bp_16 RF0LL:1;                      
    bp_16 RF1NL:1;                      
    bp_16 RF1WL:1;                      
    bp_16 RF1FL:1;                      
    bp_16 RF1LL:1;                      
    bp_16 HPML:1;                       
    bp_16 TCL:1;                        
    bp_16 TCFL:1;                       
    bp_16 TFEL:1;                       
    bp_16 TEFNL:1;                      
    bp_16 TEFWL:1;                      
    bp_16 TEFFL:1;                      
    bp_16 TEFLL:1;                      
    bp_32 TSWL:1;                       
    bp_32 MRAFL:1;                      
    bp_32 TOOL:1;                       
    bp_32 DRXL:1;                       
    bp_32 BECL:1;                       
    bp_32 BEUL:1;                       
    bp_32 ELOL:1;                       
    bp_32 EPL:1;                        
    bp_32 EWL:1;                        
    bp_32 BOL:1;                        
    bp_32 WDIL:1;                       
    bp_32 PEAL:1;                       
    bp_32 PEDL:1;                       
    bp_32 ARAL:1;                       
    bp_32 rsvd1:2;                      
};

union MCAN_ILS_REG {
    bp_32  all;
    struct  MCAN_ILS_BITS  bit;
};

struct MCAN_ILE_BITS {                  
    bp_16 EINT0:1;                      
    bp_16 EINT1:1;                      
    bp_16 rsvd1:14;                     
    bp_32 rsvd2:16;                     
};

union MCAN_ILE_REG {
    bp_32  all;
    struct  MCAN_ILE_BITS  bit;
};

struct MCAN_GFC_BITS {                  
    bp_16 RRFE:1;                       
    bp_16 RRFS:1;                       
    bp_16 ANFE:2;                       
    bp_16 ANFS:2;                       
    bp_16 rsvd1:10;                     
    bp_32 rsvd2:16;                     
};

union MCAN_GFC_REG {
    bp_32  all;
    struct  MCAN_GFC_BITS  bit;
};

struct MCAN_SIDFC_BITS {                
    bp_16 rsvd1:2;                      
    bp_16 FLSSA:14;                     
    bp_32 LSS:8;                        
    bp_32 rsvd2:8;                      
};

union MCAN_SIDFC_REG {
    bp_32  all;
    struct  MCAN_SIDFC_BITS  bit;
};

struct MCAN_XIDFC_BITS {                
    bp_16 rsvd1:2;                      
    bp_16 FLESA:14;                     
    bp_32 LSE:7;                        
    bp_32 rsvd2:9;                      
};

union MCAN_XIDFC_REG {
    bp_32  all;
    struct  MCAN_XIDFC_BITS  bit;
};

struct MCAN_XIDAM_BITS {                
    bp_32 EIDM:29;                      
    bp_32 rsvd1:3;                      
};

union MCAN_XIDAM_REG {
    bp_32  all;
    struct  MCAN_XIDAM_BITS  bit;
};

struct MCAN_HPMS_BITS {                 
    bp_16 BIDX:6;                       
    bp_16 MSI:2;                        
    bp_16 FIDX:7;                       
    bp_16 FLST:1;                       
    bp_32 rsvd1:16;                     
};

union MCAN_HPMS_REG {
    bp_32  all;
    struct  MCAN_HPMS_BITS  bit;
};

struct MCAN_NDAT1_BITS {                
    bp_16 ND0:1;                        
    bp_16 ND1:1;                        
    bp_16 ND2:1;                        
    bp_16 ND3:1;                        
    bp_16 ND4:1;                        
    bp_16 ND5:1;                        
    bp_16 ND6:1;                        
    bp_16 ND7:1;                        
    bp_16 ND8:1;                        
    bp_16 ND9:1;                        
    bp_16 ND10:1;                       
    bp_16 ND11:1;                       
    bp_16 ND12:1;                       
    bp_16 ND13:1;                       
    bp_16 ND14:1;                       
    bp_16 ND15:1;                       
    bp_32 ND16:1;                       
    bp_32 ND17:1;                       
    bp_32 ND18:1;                       
    bp_32 ND19:1;                       
    bp_32 ND20:1;                       
    bp_32 ND21:1;                       
    bp_32 ND22:1;                       
    bp_32 ND23:1;                       
    bp_32 ND24:1;                       
    bp_32 ND25:1;                       
    bp_32 ND26:1;                       
    bp_32 ND27:1;                       
    bp_32 ND28:1;                       
    bp_32 ND29:1;                       
    bp_32 ND30:1;                       
    bp_32 ND31:1;                       
};

union MCAN_NDAT1_REG {
    bp_32  all;
    struct  MCAN_NDAT1_BITS  bit;
};

struct MCAN_NDAT2_BITS {                
    bp_16 ND32:1;                       
    bp_16 ND33:1;                       
    bp_16 ND34:1;                       
    bp_16 ND35:1;                       
    bp_16 ND36:1;                       
    bp_16 ND37:1;                       
    bp_16 ND38:1;                       
    bp_16 ND39:1;                       
    bp_16 ND40:1;                       
    bp_16 ND41:1;                       
    bp_16 ND42:1;                       
    bp_16 ND43:1;                       
    bp_16 ND44:1;                       
    bp_16 ND45:1;                       
    bp_16 ND46:1;                       
    bp_16 ND47:1;                       
    bp_32 ND48:1;                       
    bp_32 ND49:1;                       
    bp_32 ND50:1;                       
    bp_32 ND51:1;                       
    bp_32 ND52:1;                       
    bp_32 ND53:1;                       
    bp_32 ND54:1;                       
    bp_32 ND55:1;                       
    bp_32 ND56:1;                       
    bp_32 ND57:1;                       
    bp_32 ND58:1;                       
    bp_32 ND59:1;                       
    bp_32 ND60:1;                       
    bp_32 ND61:1;                       
    bp_32 ND62:1;                       
    bp_32 ND63:1;                       
};

union MCAN_NDAT2_REG {
    bp_32  all;
    struct  MCAN_NDAT2_BITS  bit;
};

struct MCAN_RXF0C_BITS {                
    bp_16 rsvd1:2;                      
    bp_16 F0SA:14;                      
    bp_32 F0S:7;                        
    bp_32 rsvd2:1;                      
    bp_32 F0WM:7;                       
    bp_32 F0OM:1;                       
};

union MCAN_RXF0C_REG {
    bp_32  all;
    struct  MCAN_RXF0C_BITS  bit;
};

struct MCAN_RXF0S_BITS {                
    bp_16 F0FL:7;                       
    bp_16 rsvd1:1;                      
    bp_16 F0GI:6;                       
    bp_16 rsvd2:2;                      
    bp_32 F0PI:6;                       
    bp_32 rsvd3:2;                      
    bp_32 F0F:1;                        
    bp_32 RF0L:1;                       
    bp_32 rsvd4:6;                      
};

union MCAN_RXF0S_REG {
    bp_32  all;
    struct  MCAN_RXF0S_BITS  bit;
};

struct MCAN_RXF0A_BITS {                
    bp_16 F0AI:6;                       
    bp_16 rsvd1:10;                     
    bp_32 rsvd2:16;                     
};

union MCAN_RXF0A_REG {
    bp_32  all;
    struct  MCAN_RXF0A_BITS  bit;
};

struct MCAN_RXBC_BITS {                 
    bp_16 rsvd1:2;                      
    bp_16 RBSA:14;                      
    bp_32 rsvd2:16;                     
};

union MCAN_RXBC_REG {
    bp_32  all;
    struct  MCAN_RXBC_BITS  bit;
};

struct MCAN_RXF1C_BITS {                
    bp_16 rsvd1:2;                      
    bp_16 F1SA:14;                      
    bp_32 F1S:7;                        
    bp_32 rsvd2:1;                      
    bp_32 F1WM:7;                       
    bp_32 F1OM:1;                       
};

union MCAN_RXF1C_REG {
    bp_32  all;
    struct  MCAN_RXF1C_BITS  bit;
};

struct MCAN_RXF1S_BITS {                
    bp_16 F1FL:7;                       
    bp_16 rsvd1:1;                      
    bp_16 F1GI:6;                       
    bp_16 rsvd2:2;                      
    bp_32 F1PI:6;                       
    bp_32 rsvd3:2;                      
    bp_32 F1F:1;                        
    bp_32 RF1L:1;                       
    bp_32 rsvd4:4;                      
    bp_32 DMS:2;                        
};

union MCAN_RXF1S_REG {
    bp_32  all;
    struct  MCAN_RXF1S_BITS  bit;
};

struct MCAN_RXF1A_BITS {                
    bp_16 F1AI:6;                       
    bp_16 rsvd1:10;                     
    bp_32 rsvd2:16;                     
};

union MCAN_RXF1A_REG {
    bp_32  all;
    struct  MCAN_RXF1A_BITS  bit;
};

struct MCAN_RXESC_BITS {                
    bp_16 F0DS:3;                       
    bp_16 rsvd1:1;                      
    bp_16 F1DS:3;                       
    bp_16 rsvd2:1;                      
    bp_16 RBDS:3;                       
    bp_16 rsvd3:5;                      
    bp_32 rsvd4:16;                     
};

union MCAN_RXESC_REG {
    bp_32  all;
    struct  MCAN_RXESC_BITS  bit;
};

struct MCAN_TXBC_BITS {                 
    bp_16 rsvd1:2;                      
    bp_16 TBSA:14;                      
    bp_32 NDTB:6;                       
    bp_32 rsvd2:2;                      
    bp_32 TFQS:6;                       
    bp_32 TFQM:1;                       
    bp_32 rsvd3:1;                      
};

union MCAN_TXBC_REG {
    bp_32  all;
    struct  MCAN_TXBC_BITS  bit;
};

struct MCAN_TXFQS_BITS {                
    bp_16 TFFL:6;                       
    bp_16 rsvd1:2;                      
    bp_16 TFGI:5;                       
    bp_16 rsvd2:3;                      
    bp_32 TFQP:5;                       
    bp_32 TFQF:1;                       
    bp_32 rsvd3:10;                     
};

union MCAN_TXFQS_REG {
    bp_32  all;
    struct  MCAN_TXFQS_BITS  bit;
};

struct MCAN_TXESC_BITS {                
    bp_16 TBDS:3;                       
    bp_16 rsvd1:13;                     
    bp_32 rsvd2:16;                     
};

union MCAN_TXESC_REG {
    bp_32  all;
    struct  MCAN_TXESC_BITS  bit;
};

struct MCAN_TXBRP_BITS {                
    bp_16 TRP0:1;                       
    bp_16 TRP1:1;                       
    bp_16 TRP2:1;                       
    bp_16 TRP3:1;                       
    bp_16 TRP4:1;                       
    bp_16 TRP5:1;                       
    bp_16 TRP6:1;                       
    bp_16 TRP7:1;                       
    bp_16 TRP8:1;                       
    bp_16 TRP9:1;                       
    bp_16 TRP10:1;                      
    bp_16 TRP11:1;                      
    bp_16 TRP12:1;                      
    bp_16 TRP13:1;                      
    bp_16 TRP14:1;                      
    bp_16 TRP15:1;                      
    bp_32 TRP16:1;                      
    bp_32 TRP17:1;                      
    bp_32 TRP18:1;                      
    bp_32 TRP19:1;                      
    bp_32 TRP20:1;                      
    bp_32 TRP21:1;                      
    bp_32 TRP22:1;                      
    bp_32 TRP23:1;                      
    bp_32 TRP24:1;                      
    bp_32 TRP25:1;                      
    bp_32 TRP26:1;                      
    bp_32 TRP27:1;                      
    bp_32 TRP28:1;                      
    bp_32 TRP29:1;                      
    bp_32 TRP30:1;                      
    bp_32 TRP31:1;                      
};

union MCAN_TXBRP_REG {
    bp_32  all;
    struct  MCAN_TXBRP_BITS  bit;
};

struct MCAN_TXBAR_BITS {                
    bp_16 AR0:1;                        
    bp_16 AR1:1;                        
    bp_16 AR2:1;                        
    bp_16 AR3:1;                        
    bp_16 AR4:1;                        
    bp_16 AR5:1;                        
    bp_16 AR6:1;                        
    bp_16 AR7:1;                        
    bp_16 AR8:1;                        
    bp_16 AR9:1;                        
    bp_16 AR10:1;                       
    bp_16 AR11:1;                       
    bp_16 AR12:1;                       
    bp_16 AR13:1;                       
    bp_16 AR14:1;                       
    bp_16 AR15:1;                       
    bp_32 AR16:1;                       
    bp_32 AR17:1;                       
    bp_32 AR18:1;                       
    bp_32 AR19:1;                       
    bp_32 AR20:1;                       
    bp_32 AR21:1;                       
    bp_32 AR22:1;                       
    bp_32 AR23:1;                       
    bp_32 AR24:1;                       
    bp_32 AR25:1;                       
    bp_32 AR26:1;                       
    bp_32 AR27:1;                       
    bp_32 AR28:1;                       
    bp_32 AR29:1;                       
    bp_32 AR30:1;                       
    bp_32 AR31:1;                       
};

union MCAN_TXBAR_REG {
    bp_32  all;
    struct  MCAN_TXBAR_BITS  bit;
};

struct MCAN_TXBCR_BITS {                
    bp_16 CR0:1;                        
    bp_16 CR1:1;                        
    bp_16 CR2:1;                        
    bp_16 CR3:1;                        
    bp_16 CR4:1;                        
    bp_16 CR5:1;                        
    bp_16 CR6:1;                        
    bp_16 CR7:1;                        
    bp_16 CR8:1;                        
    bp_16 CR9:1;                        
    bp_16 CR10:1;                       
    bp_16 CR11:1;                       
    bp_16 CR12:1;                       
    bp_16 CR13:1;                       
    bp_16 CR14:1;                       
    bp_16 CR15:1;                       
    bp_32 CR16:1;                       
    bp_32 CR17:1;                       
    bp_32 CR18:1;                       
    bp_32 CR19:1;                       
    bp_32 CR20:1;                       
    bp_32 CR21:1;                       
    bp_32 CR22:1;                       
    bp_32 CR23:1;                       
    bp_32 CR24:1;                       
    bp_32 CR25:1;                       
    bp_32 CR26:1;                       
    bp_32 CR27:1;                       
    bp_32 CR28:1;                       
    bp_32 CR29:1;                       
    bp_32 CR30:1;                       
    bp_32 CR31:1;                       
};

union MCAN_TXBCR_REG {
    bp_32  all;
    struct  MCAN_TXBCR_BITS  bit;
};

struct MCAN_TXBTO_BITS {                
    bp_16 TO0:1;                        
    bp_16 TO1:1;                        
    bp_16 TO2:1;                        
    bp_16 TO3:1;                        
    bp_16 TO4:1;                        
    bp_16 TO5:1;                        
    bp_16 TO6:1;                        
    bp_16 TO7:1;                        
    bp_16 TO8:1;                        
    bp_16 TO9:1;                        
    bp_16 TO10:1;                       
    bp_16 TO11:1;                       
    bp_16 TO12:1;                       
    bp_16 TO13:1;                       
    bp_16 TO14:1;                       
    bp_16 TO15:1;                       
    bp_32 TO16:1;                       
    bp_32 TO17:1;                       
    bp_32 TO18:1;                       
    bp_32 TO19:1;                       
    bp_32 TO20:1;                       
    bp_32 TO21:1;                       
    bp_32 TO22:1;                       
    bp_32 TO23:1;                       
    bp_32 TO24:1;                       
    bp_32 TO25:1;                       
    bp_32 TO26:1;                       
    bp_32 TO27:1;                       
    bp_32 TO28:1;                       
    bp_32 TO29:1;                       
    bp_32 TO30:1;                       
    bp_32 TO31:1;                       
};

union MCAN_TXBTO_REG {
    bp_32  all;
    struct  MCAN_TXBTO_BITS  bit;
};

struct MCAN_TXBCF_BITS {                
    bp_16 CF0:1;                        
    bp_16 CF1:1;                        
    bp_16 CF2:1;                        
    bp_16 CF3:1;                        
    bp_16 CF4:1;                        
    bp_16 CF5:1;                        
    bp_16 CF6:1;                        
    bp_16 CF7:1;                        
    bp_16 CF8:1;                        
    bp_16 CF9:1;                        
    bp_16 CF10:1;                       
    bp_16 CF11:1;                       
    bp_16 CF12:1;                       
    bp_16 CF13:1;                       
    bp_16 CF14:1;                       
    bp_16 CF15:1;                       
    bp_32 CF16:1;                       
    bp_32 CF17:1;                       
    bp_32 CF18:1;                       
    bp_32 CF19:1;                       
    bp_32 CF20:1;                       
    bp_32 CF21:1;                       
    bp_32 CF22:1;                       
    bp_32 CF23:1;                       
    bp_32 CF24:1;                       
    bp_32 CF25:1;                       
    bp_32 CF26:1;                       
    bp_32 CF27:1;                       
    bp_32 CF28:1;                       
    bp_32 CF29:1;                       
    bp_32 CF30:1;                       
    bp_32 CF31:1;                       
};

union MCAN_TXBCF_REG {
    bp_32  all;
    struct  MCAN_TXBCF_BITS  bit;
};

struct MCAN_TXBTIE_BITS {               
    bp_16 TIE0:1;                       
    bp_16 TIE1:1;                       
    bp_16 TIE2:1;                       
    bp_16 TIE3:1;                       
    bp_16 TIE4:1;                       
    bp_16 TIE5:1;                       
    bp_16 TIE6:1;                       
    bp_16 TIE7:1;                       
    bp_16 TIE8:1;                       
    bp_16 TIE9:1;                       
    bp_16 TIE10:1;                      
    bp_16 TIE11:1;                      
    bp_16 TIE12:1;                      
    bp_16 TIE13:1;                      
    bp_16 TIE14:1;                      
    bp_16 TIE15:1;                      
    bp_32 TIE16:1;                      
    bp_32 TIE17:1;                      
    bp_32 TIE18:1;                      
    bp_32 TIE19:1;                      
    bp_32 TIE20:1;                      
    bp_32 TIE21:1;                      
    bp_32 TIE22:1;                      
    bp_32 TIE23:1;                      
    bp_32 TIE24:1;                      
    bp_32 TIE25:1;                      
    bp_32 TIE26:1;                      
    bp_32 TIE27:1;                      
    bp_32 TIE28:1;                      
    bp_32 TIE29:1;                      
    bp_32 TIE30:1;                      
    bp_32 TIE31:1;                      
};

union MCAN_TXBTIE_REG {
    bp_32  all;
    struct  MCAN_TXBTIE_BITS  bit;
};

struct MCAN_TXBCIE_BITS {               
    bp_16 CFIE0:1;                      
    bp_16 CFIE1:1;                      
    bp_16 CFIE2:1;                      
    bp_16 CFIE3:1;                      
    bp_16 CFIE4:1;                      
    bp_16 CFIE5:1;                      
    bp_16 CFIE6:1;                      
    bp_16 CFIE7:1;                      
    bp_16 CFIE8:1;                      
    bp_16 CFIE9:1;                      
    bp_16 CFIE10:1;                     
    bp_16 CFIE11:1;                     
    bp_16 CFIE12:1;                     
    bp_16 CFIE13:1;                     
    bp_16 CFIE14:1;                     
    bp_16 CFIE15:1;                     
    bp_32 CFIE16:1;                     
    bp_32 CFIE17:1;                     
    bp_32 CFIE18:1;                     
    bp_32 CFIE19:1;                     
    bp_32 CFIE20:1;                     
    bp_32 CFIE21:1;                     
    bp_32 CFIE22:1;                     
    bp_32 CFIE23:1;                     
    bp_32 CFIE24:1;                     
    bp_32 CFIE25:1;                     
    bp_32 CFIE26:1;                     
    bp_32 CFIE27:1;                     
    bp_32 CFIE28:1;                     
    bp_32 CFIE29:1;                     
    bp_32 CFIE30:1;                     
    bp_32 CFIE31:1;                     
};

union MCAN_TXBCIE_REG {
    bp_32  all;
    struct  MCAN_TXBCIE_BITS  bit;
};

struct MCAN_TXEFC_BITS {                
    bp_16 rsvd1:2;                      
    bp_16 EFSA:14;                      
    bp_32 EFS:6;                        
    bp_32 rsvd2:2;                      
    bp_32 EFWM:6;                       
    bp_32 rsvd3:2;                      
};

union MCAN_TXEFC_REG {
    bp_32  all;
    struct  MCAN_TXEFC_BITS  bit;
};

struct MCAN_TXEFS_BITS {                
    bp_16 EFFL:6;                       
    bp_16 rsvd1:2;                      
    bp_16 EFGI:5;                       
    bp_16 rsvd2:3;                      
    bp_32 EFPI:5;                       
    bp_32 rsvd3:3;                      
    bp_32 EFF:1;                        
    bp_32 TEFL:1;                       
    bp_32 rsvd4:6;                      
};

union MCAN_TXEFS_REG {
    bp_32  all;
    struct  MCAN_TXEFS_BITS  bit;
};

struct MCAN_TXEFA_BITS {                
    bp_16 EFAI:5;                       
    bp_16 rsvd1:11;                     
    bp_32 rsvd2:16;                     
};

union MCAN_TXEFA_REG {
    bp_32  all;
    struct  MCAN_TXEFA_BITS  bit;
};

struct MCAN_REGS {
    union   MCAN_CREL_REG                    MCAN_CREL;                    
    bp_32                                    MCAN_ENDN;                    
    uint32_t                                 rsvd1[2];                     
    union   MCAN_DBTP_REG                    MCAN_DBTP;                    
    union   MCAN_TEST_REG                    MCAN_TEST;                    
    union   MCAN_RWD_REG                     MCAN_RWD;                     
    union   MCAN_CCCR_REG                    MCAN_CCCR;                    
    union   MCAN_NBTP_REG                    MCAN_NBTP;                    
    union   MCAN_TSCC_REG                    MCAN_TSCC;                    
    union   MCAN_TSCV_REG                    MCAN_TSCV;                    
    union   MCAN_TOCC_REG                    MCAN_TOCC;                    
    union   MCAN_TOCV_REG                    MCAN_TOCV;                    
    uint32_t                                 rsvd2[8];                     
    union   MCAN_ECR_REG                     MCAN_ECR;                     
    union   MCAN_PSR_REG                     MCAN_PSR;                     
    union   MCAN_TDCR_REG                    MCAN_TDCR;                    
    uint32_t                                 rsvd3[2];                     
    union   MCAN_IR_REG                      MCAN_IR;                      
    union   MCAN_IE_REG                      MCAN_IE;                      
    union   MCAN_ILS_REG                     MCAN_ILS;                     
    union   MCAN_ILE_REG                     MCAN_ILE;                     
    uint32_t                                 rsvd4[16];                    
    union   MCAN_GFC_REG                     MCAN_GFC;                     
    union   MCAN_SIDFC_REG                   MCAN_SIDFC;                   
    union   MCAN_XIDFC_REG                   MCAN_XIDFC;                   
    uint32_t                                 rsvd5[2];                     
    union   MCAN_XIDAM_REG                   MCAN_XIDAM;                   
    union   MCAN_HPMS_REG                    MCAN_HPMS;                    
    union   MCAN_NDAT1_REG                   MCAN_NDAT1;                   
    union   MCAN_NDAT2_REG                   MCAN_NDAT2;                   
    union   MCAN_RXF0C_REG                   MCAN_RXF0C;                   
    union   MCAN_RXF0S_REG                   MCAN_RXF0S;                   
    union   MCAN_RXF0A_REG                   MCAN_RXF0A;                   
    union   MCAN_RXBC_REG                    MCAN_RXBC;                    
    union   MCAN_RXF1C_REG                   MCAN_RXF1C;                   
    union   MCAN_RXF1S_REG                   MCAN_RXF1S;                   
    union   MCAN_RXF1A_REG                   MCAN_RXF1A;                   
    union   MCAN_RXESC_REG                   MCAN_RXESC;                   
    union   MCAN_TXBC_REG                    MCAN_TXBC;                    
    union   MCAN_TXFQS_REG                   MCAN_TXFQS;                   
    union   MCAN_TXESC_REG                   MCAN_TXESC;                   
    union   MCAN_TXBRP_REG                   MCAN_TXBRP;                   
    union   MCAN_TXBAR_REG                   MCAN_TXBAR;                   
    union   MCAN_TXBCR_REG                   MCAN_TXBCR;                   
    union   MCAN_TXBTO_REG                   MCAN_TXBTO;                   
    union   MCAN_TXBCF_REG                   MCAN_TXBCF;                   
    union   MCAN_TXBTIE_REG                  MCAN_TXBTIE;                  
    union   MCAN_TXBCIE_REG                  MCAN_TXBCIE;                  
    uint32_t                                 rsvd6[4];                     
    union   MCAN_TXEFC_REG                   MCAN_TXEFC;                   
    union   MCAN_TXEFS_REG                   MCAN_TXEFS;                   
    union   MCAN_TXEFA_REG                   MCAN_TXEFA;                   
};

struct MCANERR_REV_BITS {               
    bp_16 REVMIN:6;                     
    bp_16 rsvd1:2;                      
    bp_16 REVMAJ:3;                     
    bp_16 rsvd2:5;                      
    bp_32 MODULE_ID:12;                 
    bp_32 rsvd3:2;                      
    bp_32 SCHEME:2;                     
};

union MCANERR_REV_REG {
    bp_32  all;
    struct  MCANERR_REV_BITS  bit;
};

struct MCANERR_VECTOR_BITS {            
    bp_16 ECC_VECTOR:11;                
    bp_16 rsvd1:4;                      
    bp_16 RD_SVBUS:1;                   
    bp_32 RD_SVBUS_ADDRESS:8;           
    bp_32 RD_SVBUS_DONE:1;              
    bp_32 rsvd2:7;                      
};

union MCANERR_VECTOR_REG {
    bp_32  all;
    struct  MCANERR_VECTOR_BITS  bit;
};

struct MCANERR_STAT_BITS {              
    bp_16 NUM_RAMS:11;                  
    bp_16 rsvd1:5;                      
    bp_32 rsvd2:16;                     
};

union MCANERR_STAT_REG {
    bp_32  all;
    struct  MCANERR_STAT_BITS  bit;
};

struct MCANERR_WRAP_REV_BITS {          
    bp_16 REVMIN:6;                     
    bp_16 rsvd1:2;                      
    bp_16 REVMAJ:3;                     
    bp_16 rsvd2:5;                      
    bp_32 MODULE_ID:12;                 
    bp_32 rsvd3:2;                      
    bp_32 SCHEME:2;                     
};

union MCANERR_WRAP_REV_REG {
    bp_32  all;
    struct  MCANERR_WRAP_REV_BITS  bit;
};

struct MCANERR_CTRL_BITS {              
    bp_16 ECC_ENABLE:1;                 
    bp_16 ECC_CHECK:1;                  
    bp_16 ENABLE_RMW:1;                 
    bp_16 FORCE_SEC:1;                  
    bp_16 FORCE_DED:1;                  
    bp_16 FORCE_N_ROW:1;                
    bp_16 ERROR_ONCE:1;                 
    bp_16 rsvd1:1;                      
    bp_16 CHECK_SVBUS_TIMEOUT:1;        
    bp_16 rsvd2:7;                      
    bp_32 rsvd3:16;                     
};

union MCANERR_CTRL_REG {
    bp_32  all;
    struct  MCANERR_CTRL_BITS  bit;
};

struct MCANERR_ERR_CTRL2_BITS {         
    bp_16 ECC_BIT1:16;                  
    bp_32 ECC_BIT2:16;                  
};

union MCANERR_ERR_CTRL2_REG {
    bp_32  all;
    struct  MCANERR_ERR_CTRL2_BITS  bit;
};

struct MCANERR_ERR_STAT1_BITS {         
    bp_16 ECC_SEC:2;                    
    bp_16 ECC_DED:2;                    
    bp_16 ECC_OTHER:1;                  
    bp_16 rsvd1:2;                      
    bp_16 CTRL_REG_ERROR:1;             
    bp_16 CLR_ECC_SEC:2;                
    bp_16 CLR_ECC_DED:2;                
    bp_16 CLR_ECC_OTHER:1;              
    bp_16 rsvd2:2;                      
    bp_16 CLR_CTRL_REG_ERROR:1;         
    bp_32 ECC_BIT1:16;                  
};

union MCANERR_ERR_STAT1_REG {
    bp_32  all;
    struct  MCANERR_ERR_STAT1_BITS  bit;
};

struct MCANERR_ERR_STAT3_BITS {         
    bp_16 WB_PEND:1;                    
    bp_16 SVBUS_TIMEOUT:1;              
    bp_16 rsvd1:7;                      
    bp_16 CLR_SVBUS_TIMEOUT:1;          
    bp_16 rsvd2:6;                      
    bp_32 rsvd3:16;                     
};

union MCANERR_ERR_STAT3_REG {
    bp_32  all;
    struct  MCANERR_ERR_STAT3_BITS  bit;
};

struct MCANERR_SEC_EOI_BITS {           
    bp_16 EOI_WR:1;                     
    bp_16 rsvd1:15;                     
    bp_32 rsvd2:16;                     
};

union MCANERR_SEC_EOI_REG {
    bp_32  all;
    struct  MCANERR_SEC_EOI_BITS  bit;
};

struct MCANERR_SEC_STATUS_BITS {        
    bp_16 MSGMEM_PEND:1;                
    bp_16 rsvd1:1;                      
    bp_16 rsvd2:14;                     
    bp_32 rsvd3:16;                     
};

union MCANERR_SEC_STATUS_REG {
    bp_32  all;
    struct  MCANERR_SEC_STATUS_BITS  bit;
};

struct MCANERR_SEC_ENABLE_SET_BITS {    
    bp_16 MSGMEM_ENABLE_SET:1;          
    bp_16 rsvd1:1;                      
    bp_16 rsvd2:14;                     
    bp_32 rsvd3:16;                     
};

union MCANERR_SEC_ENABLE_SET_REG {
    bp_32  all;
    struct  MCANERR_SEC_ENABLE_SET_BITS  bit;
};

struct MCANERR_SEC_ENABLE_CLR_BITS {    
    bp_16 MSGMEM_ENABLE_CLR:1;          
    bp_16 rsvd1:1;                      
    bp_16 rsvd2:14;                     
    bp_32 rsvd3:16;                     
};

union MCANERR_SEC_ENABLE_CLR_REG {
    bp_32  all;
    struct  MCANERR_SEC_ENABLE_CLR_BITS  bit;
};

struct MCANERR_DED_EOI_BITS {           
    bp_16 EOI_WR:1;                     
    bp_16 rsvd1:15;                     
    bp_32 rsvd2:16;                     
};

union MCANERR_DED_EOI_REG {
    bp_32  all;
    struct  MCANERR_DED_EOI_BITS  bit;
};

struct MCANERR_DED_STATUS_BITS {        
    bp_16 MSGMEM_PEND:1;                
    bp_16 rsvd1:1;                      
    bp_16 rsvd2:14;                     
    bp_32 rsvd3:16;                     
};

union MCANERR_DED_STATUS_REG {
    bp_32  all;
    struct  MCANERR_DED_STATUS_BITS  bit;
};

struct MCANERR_DED_ENABLE_SET_BITS {    
    bp_16 MSGMEM_ENABLE_SET:1;          
    bp_16 rsvd1:1;                      
    bp_16 rsvd2:14;                     
    bp_32 rsvd3:16;                     
};

union MCANERR_DED_ENABLE_SET_REG {
    bp_32  all;
    struct  MCANERR_DED_ENABLE_SET_BITS  bit;
};

struct MCANERR_DED_ENABLE_CLR_BITS {    
    bp_16 MSGMEM_ENABLE_CLR:1;          
    bp_16 rsvd1:1;                      
    bp_16 rsvd2:14;                     
    bp_32 rsvd3:16;                     
};

union MCANERR_DED_ENABLE_CLR_REG {
    bp_32  all;
    struct  MCANERR_DED_ENABLE_CLR_BITS  bit;
};

struct MCANERR_AGGR_ENABLE_SET_BITS {   
    bp_16 ENABLE_PARITY_SET:1;          
    bp_16 ENABLE_TIMEOUT_SET:1;         
    bp_16 rsvd1:14;                     
    bp_32 rsvd2:16;                     
};

union MCANERR_AGGR_ENABLE_SET_REG {
    bp_32  all;
    struct  MCANERR_AGGR_ENABLE_SET_BITS  bit;
};

struct MCANERR_AGGR_ENABLE_CLR_BITS {   
    bp_16 ENABLE_PARITY_CLR:1;          
    bp_16 ENABLE_TIMEOUT_CLR:1;         
    bp_16 rsvd1:14;                     
    bp_32 rsvd2:16;                     
};

union MCANERR_AGGR_ENABLE_CLR_REG {
    bp_32  all;
    struct  MCANERR_AGGR_ENABLE_CLR_BITS  bit;
};

struct MCANERR_AGGR_STATUS_SET_BITS {   
    bp_16 AGGR_PARITY_ERR:2;            
    bp_16 SVBUS_TIMEOUT:2;              
    bp_16 rsvd1:12;                     
    bp_32 rsvd2:16;                     
};

union MCANERR_AGGR_STATUS_SET_REG {
    bp_32  all;
    struct  MCANERR_AGGR_STATUS_SET_BITS  bit;
};

struct MCANERR_AGGR_STATUS_CLR_BITS {   
    bp_16 AGGR_PARITY_ERR:2;            
    bp_16 SVBUS_TIMEOUT:2;              
    bp_16 rsvd1:12;                     
    bp_32 rsvd2:16;                     
};

union MCANERR_AGGR_STATUS_CLR_REG {
    bp_32  all;
    struct  MCANERR_AGGR_STATUS_CLR_BITS  bit;
};

struct MCAN_ERROR_REGS {
    union   MCANERR_REV_REG                  MCANERR_REV;                  
    uint32_t                                 rsvd1[2];                     
    union   MCANERR_VECTOR_REG               MCANERR_VECTOR;               
    union   MCANERR_STAT_REG                 MCANERR_STAT;                 
    union   MCANERR_WRAP_REV_REG             MCANERR_WRAP_REV;             
    union   MCANERR_CTRL_REG                 MCANERR_CTRL;                 
    bp_32                                    MCANERR_ERR_CTRL1;            
    union   MCANERR_ERR_CTRL2_REG            MCANERR_ERR_CTRL2;            
    union   MCANERR_ERR_STAT1_REG            MCANERR_ERR_STAT1;            
    bp_32                                    MCANERR_ERR_STAT2;            
    union   MCANERR_ERR_STAT3_REG            MCANERR_ERR_STAT3;            
    uint32_t                                 rsvd2[8];                     
    union   MCANERR_SEC_EOI_REG              MCANERR_SEC_EOI;              
    union   MCANERR_SEC_STATUS_REG           MCANERR_SEC_STATUS;           
    uint32_t                                 rsvd3[30];                    
    union   MCANERR_SEC_ENABLE_SET_REG       MCANERR_SEC_ENABLE_SET;       
    uint32_t                                 rsvd4[30];                    
    union   MCANERR_SEC_ENABLE_CLR_REG       MCANERR_SEC_ENABLE_CLR;       
    uint32_t                                 rsvd5[60];                    
    union   MCANERR_DED_EOI_REG              MCANERR_DED_EOI;              
    union   MCANERR_DED_STATUS_REG           MCANERR_DED_STATUS;           
    uint32_t                                 rsvd6[30];                    
    union   MCANERR_DED_ENABLE_SET_REG       MCANERR_DED_ENABLE_SET;       
    uint32_t                                 rsvd7[30];                    
    union   MCANERR_DED_ENABLE_CLR_REG       MCANERR_DED_ENABLE_CLR;       
    uint32_t                                 rsvd8[30];                    
    union   MCANERR_AGGR_ENABLE_SET_REG      MCANERR_AGGR_ENABLE_SET;      
    union   MCANERR_AGGR_ENABLE_CLR_REG      MCANERR_AGGR_ENABLE_CLR;      
    union   MCANERR_AGGR_STATUS_SET_REG      MCANERR_AGGR_STATUS_SET;      
    union   MCANERR_AGGR_STATUS_CLR_REG      MCANERR_AGGR_STATUS_CLR;      
};




extern volatile struct MCANSS_REGS McanaSsRegs;
extern volatile struct MCAN_REGS McanaRegs;
extern volatile struct MCAN_ERROR_REGS McanaErrorRegs;


























































































































 





































































extern void EnableInterrupts(void);
extern void InitPeripheralClocks(void);
extern void DisablePeripheralClocks(void);
extern void InitPieCtrl(void);
extern void InitPieVectTable(void);
extern void InitSpi(void);
extern void InitSpiGpio(void);
extern void InitSpiaGpio(void);
extern void InitSysCtrl(void);
extern void InitSysPll(Uint16 clock_source, Uint16 imult, Uint32 refdiv, Uint32 odiv,
                       Uint16 divsel, Uint32 dccbase);
extern _Bool IsPLLValid(Uint32 base, Uint16 oscSource, Uint16 pllclk, Uint16 imult,
                       Uint16 odiv, Uint16 refdiv);
extern void ComputeCntrSeedValue(Uint32 base, float fclk1_0ratio, Uint32 tolerance);




extern void ServiceDog(void);
extern void DisableDog(void);

extern Uint16 CsmUnlock(void);
extern void SysIntOsc1Sel (void);
extern void SysIntOsc2Sel (void);
extern void SysXtalOscSel (void);
extern void SysXtalOscSESel (void);






extern void InitFlash(void);
extern void FlashOff(void);




 
void IDLE(void);
void HALT(void);




extern void SetVREF(int module, int mode, int ref);




extern void DMAInitialize(void);




extern void DMACH1AddrConfig(volatile Uint16 *DMA_Dest,
                             volatile Uint16 *DMA_Source);
extern void DMACH1AddrConfig32bit(volatile Uint32 *DMA_Dest,
                                  volatile Uint32 *DMA_Source);
extern void DMACH1BurstConfig(Uint16 bsize, int16 srcbstep,
                              int16 desbstep);
extern void DMACH1TransferConfig(Uint16 tsize, int16 srctstep,
                                 int16 deststep);
extern void DMACH1WrapConfig(Uint16 srcwsize, int16 srcwstep,
                             Uint16 deswsize, int16 deswstep);
extern void DMACH1ModeConfig(Uint16 persel, Uint16 perinte,
                             Uint16 oneshot, Uint16 cont, Uint16 synce,
                             Uint16 syncsel, Uint16 ovrinte,
                             Uint16 datasize, Uint16 chintmode,
                             Uint16 chinte);
extern void StartDMACH1(void);




extern void DMACH2AddrConfig(volatile Uint16 *DMA_Dest,
                             volatile Uint16 *DMA_Source);
extern void DMACH2AddrConfig32bit(volatile Uint32 *DMA_Dest,
                                  volatile Uint32 *DMA_Source);
extern void DMACH2BurstConfig(Uint16 bsize, int16 srcbstep, int16 desbstep);
extern void DMACH2TransferConfig(Uint16 tsize, int16 srctstep,
                                 int16 deststep);
extern void DMACH2WrapConfig(Uint16 srcwsize, int16 srcwstep,
                             Uint16 deswsize, int16 deswstep);
extern void DMACH2ModeConfig(Uint16 persel, Uint16 perinte,
                             Uint16 oneshot, Uint16 cont,
                             Uint16 synce, Uint16 syncsel,
                             Uint16 ovrinte, Uint16 datasize,
                             Uint16 chintmode, Uint16 chinte);
extern void StartDMACH2(void);




extern void DMACH3AddrConfig(volatile Uint16 *DMA_Dest,
                             volatile Uint16 *DMA_Source);
extern void DMACH3AddrConfig32bit(volatile Uint32 *DMA_Dest,
                                  volatile Uint32 *DMA_Source);
extern void DMACH3BurstConfig(Uint16 bsize, int16 srcbstep, int16 desbstep);
extern void DMACH3TransferConfig(Uint16 tsize, int16 srctstep,
                                 int16 deststep);
extern void DMACH3WrapConfig(Uint16 srcwsize, int16 srcwstep,
                             Uint16 deswsize, int16 deswstep);
extern void DMACH3ModeConfig(Uint16 persel, Uint16 perinte,
                             Uint16 oneshot, Uint16 cont, Uint16 synce,
                             Uint16 syncsel, Uint16 ovrinte,
                             Uint16 datasize, Uint16 chintmode,
                             Uint16 chinte);
extern void StartDMACH3(void);




extern void DMACH4AddrConfig(volatile Uint16 *DMA_Dest,
                             volatile Uint16 *DMA_Source);
extern void DMACH4AddrConfig32bit(volatile Uint32 *DMA_Dest,
                                  volatile Uint32 *DMA_Source);
extern void DMACH4BurstConfig(Uint16 bsize, int16 srcbstep, int16 desbstep);
extern void DMACH4TransferConfig(Uint16 tsize, int16 srctstep,
                                 int16 deststep);
extern void DMACH4WrapConfig(Uint16 srcwsize, int16 srcwstep,
                             Uint16 deswsize, int16 deswstep);
extern void DMACH4ModeConfig(Uint16 persel, Uint16 perinte,
                             Uint16 oneshot, Uint16 cont, Uint16 synce,
                             Uint16 syncsel, Uint16 ovrinte,
                             Uint16 datasize, Uint16 chintmode,
                             Uint16 chinte);
extern void StartDMACH4(void);




extern void DMACH5AddrConfig(volatile Uint16 *DMA_Dest,
                             volatile Uint16 *DMA_Source);
extern void DMACH5AddrConfig32bit(volatile Uint32 *DMA_Dest,
                                  volatile Uint32 *DMA_Source);
extern void DMACH5BurstConfig(Uint16 bsize, int16 srcbstep, int16 desbstep);
extern void DMACH5TransferConfig(Uint16 tsize, int16 srctstep,
                                 int16 deststep);
extern void DMACH5WrapConfig(Uint16 srcwsize, int16 srcwstep,
                             Uint16 deswsize, int16 deswstep);
extern void DMACH5ModeConfig(Uint16 persel, Uint16 perinte,
                             Uint16 oneshot, Uint16 cont, Uint16 synce,
                             Uint16 syncsel, Uint16 ovrinte,
                             Uint16 datasize, Uint16 chintmode,
                             Uint16 chinte);
extern void StartDMACH5(void);




extern void DMACH6AddrConfig(volatile Uint16 *DMA_Dest,
                             volatile Uint16 *DMA_Source);
extern void DMACH6AddrConfig32bit(volatile Uint32 *DMA_Dest,
                                  volatile Uint32 *DMA_Source);
extern void DMACH6BurstConfig(Uint16 bsize, int16 srcbstep, int16 desbstep);
extern void DMACH6TransferConfig(Uint16 tsize, int16 srctstep,
                                 int16 deststep);
extern void DMACH6WrapConfig(Uint16 srcwsize, int16 srcwstep,
                             Uint16 deswsize, int16 deswstep);
extern void DMACH6ModeConfig(Uint16 persel, Uint16 perinte,
                             Uint16 oneshot, Uint16 cont, Uint16 synce,
                             Uint16 syncsel, Uint16 ovrinte,
                             Uint16 datasize, Uint16 chintmode,
                             Uint16 chinte);
extern void StartDMACH6(void);




extern void InitGpio(void);
extern void GPIO_SetupPinMux(Uint16 gpioNumber, Uint16 cpu,
                             Uint16 muxPosition);
extern void GPIO_SetupPinOptions(Uint16 gpioNumber, Uint16 output,
                                 Uint16 flags);
extern void GPIO_SetupLock(Uint16 gpioNumber, Uint16 flags);
extern void GPIO_SetupXINT1Gpio(Uint16 gpioNumber);
extern void GPIO_SetupXINT2Gpio(Uint16 gpioNumber);
extern void GPIO_SetupXINT3Gpio(Uint16 gpioNumber);
extern void GPIO_SetupXINT4Gpio(Uint16 gpioNumber);
extern void GPIO_SetupXINT5Gpio(Uint16 gpioNumber);
Uint16 GPIO_ReadPin(Uint16 gpioNumber);
void GPIO_WritePin(Uint16 gpioNumber, Uint16 outVal);






extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsLoadSize;
extern Uint16 RamfuncsRunStart;
extern Uint16 RamfuncsRunEnd;
extern Uint16 RamfuncsRunSize;







                                           






















































































































struct CPUTIMER_VARS {
   volatile struct  CPUTIMER_REGS  *RegsAddr;
   Uint32    InterruptCount;
   float   CPUFreqInMHz;
   float   PeriodInUSec;
};




void InitCpuTimers(void);
void ConfigCpuTimer(struct CPUTIMER_VARS *Timer, float Freq, float Period);

extern struct CPUTIMER_VARS CpuTimer0;
extern struct CPUTIMER_VARS CpuTimer1;
extern struct CPUTIMER_VARS CpuTimer2;





































































































































































































































































































































































































































































































































































































































































interrupt void TIMER1_ISR(void);
interrupt void TIMER2_ISR(void);
interrupt void DATALOG_ISR(void);
interrupt void RTOS_ISR(void);
interrupt void EMU_ISR(void);
interrupt void NMI_ISR(void);
interrupt void ILLEGAL_ISR(void);
interrupt void USER1_ISR(void);
interrupt void USER2_ISR(void);
interrupt void USER3_ISR(void);
interrupt void USER4_ISR(void);
interrupt void USER5_ISR(void);
interrupt void USER6_ISR(void);
interrupt void USER7_ISR(void);
interrupt void USER8_ISR(void);
interrupt void USER9_ISR(void);
interrupt void USER10_ISR(void);
interrupt void USER11_ISR(void);
interrupt void USER12_ISR(void);
interrupt void ADCA1_ISR(void);
interrupt void ADCB1_ISR(void);
interrupt void ADCC1_ISR(void);
interrupt void XINT1_ISR(void);
interrupt void XINT2_ISR(void);
interrupt void TIMER0_ISR(void);
interrupt void WAKE_ISR(void);
interrupt void EPWM1_TZ_ISR(void);
interrupt void EPWM2_TZ_ISR(void);
interrupt void EPWM3_TZ_ISR(void);
interrupt void EPWM4_TZ_ISR(void);
interrupt void EPWM5_TZ_ISR(void);
interrupt void EPWM6_TZ_ISR(void);
interrupt void EPWM7_TZ_ISR(void);
interrupt void EPWM8_TZ_ISR(void);
interrupt void EPWM1_ISR(void);
interrupt void EPWM2_ISR(void);
interrupt void EPWM3_ISR(void);
interrupt void EPWM4_ISR(void);
interrupt void EPWM5_ISR(void);
interrupt void EPWM6_ISR(void);
interrupt void EPWM7_ISR(void);
interrupt void EPWM8_ISR(void);
interrupt void ECAP1_ISR(void);
interrupt void ECAP2_ISR(void);
interrupt void ECAP3_ISR(void);
interrupt void EQEP1_ISR(void);
interrupt void EQEP2_ISR(void);
interrupt void CLB1_ISR(void);
interrupt void CLB2_ISR(void);
interrupt void CLB3_ISR(void);
interrupt void CLB4_ISR(void);
interrupt void SPIA_RX_ISR(void);
interrupt void SPIA_TX_ISR(void);
interrupt void SPIB_RX_ISR(void);
interrupt void SPIB_TX_ISR(void);
interrupt void DMA_CH1_ISR(void);
interrupt void DMA_CH2_ISR(void);
interrupt void DMA_CH3_ISR(void);
interrupt void DMA_CH4_ISR(void);
interrupt void DMA_CH5_ISR(void);
interrupt void DMA_CH6_ISR(void);
interrupt void I2CA_ISR(void);
interrupt void I2CA_FIFO_ISR(void);
interrupt void I2CB_ISR(void);
interrupt void I2CB_FIFO_ISR(void);
interrupt void SCIA_RX_ISR(void);
interrupt void SCIA_TX_ISR(void);
interrupt void SCIB_RX_ISR(void);
interrupt void SCIB_TX_ISR(void);
interrupt void CANA0_ISR(void);
interrupt void CANA1_ISR(void);
interrupt void ADCA_EVT_ISR(void);
interrupt void ADCA2_ISR(void);
interrupt void ADCA3_ISR(void);
interrupt void ADCA4_ISR(void);
interrupt void ADCB_EVT_ISR(void);
interrupt void ADCB2_ISR(void);
interrupt void ADCB3_ISR(void);
interrupt void ADCB4_ISR(void);
interrupt void CLA1_1_ISR(void);
interrupt void CLA1_2_ISR(void);
interrupt void CLA1_3_ISR(void);
interrupt void CLA1_4_ISR(void);
interrupt void CLA1_5_ISR(void);
interrupt void CLA1_6_ISR(void);
interrupt void CLA1_7_ISR(void);
interrupt void CLA1_8_ISR(void);
interrupt void XINT3_ISR(void);
interrupt void XINT4_ISR(void);
interrupt void XINT5_ISR(void);
interrupt void MPOST_ISR(void);
interrupt void FMC_ISR(void);
interrupt void FPU_OVERFLOW_ISR(void);
interrupt void FPU_UNDERFLOW_ISR(void);
interrupt void SYS_ERR_ISR(void);
interrupt void ECAP3_2_ISR(void);
interrupt void SDFM1_ISR(void);
interrupt void SDFM2_ISR(void);
interrupt void SDFM1DR1_ISR(void);
interrupt void SDFM1DR2_ISR(void);
interrupt void SDFM1DR3_ISR(void);
interrupt void SDFM1DR4_ISR(void);
interrupt void SDFM2DR1_ISR(void);
interrupt void SDFM2DR2_ISR(void);
interrupt void SDFM2DR3_ISR(void);
interrupt void SDFM2DR4_ISR(void);
interrupt void FSITXA1_ISR(void);
interrupt void FSITXA2_ISR(void);
interrupt void FSIRXA1_ISR(void);
interrupt void FSIRXA2_ISR(void);
interrupt void DCC0_ISR(void);
interrupt void LINA_0_ISR(void);
interrupt void LINA_1_ISR(void);
interrupt void LINB_0_ISR(void);
interrupt void LINB_1_ISR(void);
interrupt void PMBUSA_ISR(void);
interrupt void DCC1_ISR(void);
interrupt void MCANA_0_ISR(void);
interrupt void MCANA_1_ISR(void);
interrupt void MCANA_ECC_ISR(void);
interrupt void MCANA_WAKE_ISR(void);
interrupt void BGCRC_ISR(void);
interrupt void HICA_ISR(void);
interrupt void ADCC_EVT_ISR(void);
interrupt void ADCC2_ISR(void);
interrupt void ADCC3_ISR(void);
interrupt void ADCC4_ISR(void);
interrupt void RAM_CORR_ERR_ISR(void);
interrupt void FLASH_CORR_ERR_ISR(void);
interrupt void RAM_ACC_VIOL_ISR(void);
interrupt void AES_SINTREQUEST_ISR(void);
interrupt void BGCRC_CLA1_ISR(void);
interrupt void CLA_OVERFLOW_ISR(void);
interrupt void CLA_UNDERFLOW_ISR(void);



interrupt void PIE_RESERVED_ISR(void);              
interrupt void EMPTY_ISR(void);                     
interrupt void NOTUSED_ISR(void);







extern void F28x_usDelay(long LoopCount);


































































#pragma CODE_SECTION(InitFlash, ".TI.ramfunc");
#pragma CODE_SECTION(FlashOff, ".TI.ramfunc");




















void InitSysCtrl(void)
{
    
    
    
    DisableDog();

    
    
    
    
    
    
    
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);

    
    
    
    
    InitFlash();

    
    
    
    
    
    
    
    
    
    
    
    InitSysPll(1, 40, 0, 3, 0, 0);


    
    
    
    InitPeripheralClocks();
}








void InitPeripheralClocks(void)
{
    __eallow();

    CpuSysRegs.PCLKCR0.bit.CLA1 = 1;
    CpuSysRegs.PCLKCR0.bit.DMA = 1;
    CpuSysRegs.PCLKCR0.bit.CPUTIMER0 = 1;
    CpuSysRegs.PCLKCR0.bit.CPUTIMER1 = 1;
    CpuSysRegs.PCLKCR0.bit.CPUTIMER2 = 1;
    CpuSysRegs.PCLKCR0.bit.CPUBGCRC = 1;
    CpuSysRegs.PCLKCR0.bit.CLA1BGCRC = 1;
    CpuSysRegs.PCLKCR0.bit.HRCAL = 1;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    CpuSysRegs.PCLKCR0.bit.ERAD = 1;

    CpuSysRegs.PCLKCR2.bit.EPWM1 = 1;
    CpuSysRegs.PCLKCR2.bit.EPWM2 = 1;
    CpuSysRegs.PCLKCR2.bit.EPWM3 = 1;
    CpuSysRegs.PCLKCR2.bit.EPWM4 = 1;
    CpuSysRegs.PCLKCR2.bit.EPWM5 = 1;
    CpuSysRegs.PCLKCR2.bit.EPWM6 = 1;
    CpuSysRegs.PCLKCR2.bit.EPWM7 = 1;
    CpuSysRegs.PCLKCR2.bit.EPWM8 = 1;

    CpuSysRegs.PCLKCR3.bit.ECAP1 = 1;
    CpuSysRegs.PCLKCR3.bit.ECAP2 = 1;
    CpuSysRegs.PCLKCR3.bit.ECAP3 = 1;

    CpuSysRegs.PCLKCR4.bit.EQEP1 = 1;
    CpuSysRegs.PCLKCR4.bit.EQEP2 = 1;

    CpuSysRegs.PCLKCR6.bit.SD1 = 1;
    CpuSysRegs.PCLKCR6.bit.SD2 = 1;

    CpuSysRegs.PCLKCR7.bit.SCI_A = 1;
    CpuSysRegs.PCLKCR7.bit.SCI_B = 1;

    CpuSysRegs.PCLKCR8.bit.SPI_A = 1;
    CpuSysRegs.PCLKCR8.bit.SPI_B = 1;

    CpuSysRegs.PCLKCR9.bit.I2C_A = 1;
    CpuSysRegs.PCLKCR9.bit.I2C_B = 1;

    CpuSysRegs.PCLKCR10.bit.CAN_A = 1;
    CpuSysRegs.PCLKCR10.bit.MCAN_A = 1;

    CpuSysRegs.PCLKCR13.bit.ADC_A = 1;
    CpuSysRegs.PCLKCR13.bit.ADC_B = 1;
    CpuSysRegs.PCLKCR13.bit.ADC_C = 1;

    CpuSysRegs.PCLKCR14.bit.CMPSS1 = 1;
    CpuSysRegs.PCLKCR14.bit.CMPSS2 = 1;
    CpuSysRegs.PCLKCR14.bit.CMPSS3 = 1;
    CpuSysRegs.PCLKCR14.bit.CMPSS4 = 1;

    CpuSysRegs.PCLKCR16.bit.DAC_A = 1;
    CpuSysRegs.PCLKCR16.bit.DAC_B = 1;

    CpuSysRegs.PCLKCR17.bit.CLB1 = 1;
    CpuSysRegs.PCLKCR17.bit.CLB2 = 1;
    CpuSysRegs.PCLKCR17.bit.CLB3 = 1;
    CpuSysRegs.PCLKCR17.bit.CLB4 = 1;

    CpuSysRegs.PCLKCR18.bit.FSITX_A = 1;
    CpuSysRegs.PCLKCR18.bit.FSIRX_A = 1;

    CpuSysRegs.PCLKCR19.bit.LIN_A = 1;
    CpuSysRegs.PCLKCR19.bit.LIN_B = 1;

    CpuSysRegs.PCLKCR20.bit.PMBUS_A = 1;

    CpuSysRegs.PCLKCR21.bit.DCC0 = 1;
    CpuSysRegs.PCLKCR21.bit.DCC1 = 1;

    CpuSysRegs.PCLKCR25.bit.HICA = 1;
    CpuSysRegs.PCLKCR26.bit.AESA = 1;
    CpuSysRegs.PCLKCR27.bit.EPG1 = 1;

    __edis();
}




void DisablePeripheralClocks(void)
{
    __eallow();

    CpuSysRegs.PCLKCR0.all = 0;
    CpuSysRegs.PCLKCR2.all = 0;
    CpuSysRegs.PCLKCR3.all = 0;
    CpuSysRegs.PCLKCR4.all = 0;
    CpuSysRegs.PCLKCR6.all = 0;
    CpuSysRegs.PCLKCR7.all = 0;
    CpuSysRegs.PCLKCR8.all = 0;
    CpuSysRegs.PCLKCR9.all = 0;
    CpuSysRegs.PCLKCR10.all = 0;
    CpuSysRegs.PCLKCR13.all = 0;
    CpuSysRegs.PCLKCR14.all = 0;
    CpuSysRegs.PCLKCR16.all = 0;
    CpuSysRegs.PCLKCR17.all = 0;
    CpuSysRegs.PCLKCR18.all = 0;
    CpuSysRegs.PCLKCR19.all = 0;
    CpuSysRegs.PCLKCR20.all = 0;
    CpuSysRegs.PCLKCR21.all = 0;
    CpuSysRegs.PCLKCR25.all = 0;
    CpuSysRegs.PCLKCR26.all = 0;
    CpuSysRegs.PCLKCR27.all = 0;

    __edis();
}








void InitFlash(void)
{
    __eallow();

    
    
    
    
    
    
    
    Flash0CtrlRegs.FPAC1.bit.PMPPWR = 0x1;
    Flash0CtrlRegs.FBFALLBACK.bit.BNKPWR0 = 0x3F;

    
    
    
    Flash0CtrlRegs.FRD_INTF_CTRL.bit.DATA_CACHE_EN = 0;
    Flash0CtrlRegs.FRD_INTF_CTRL.bit.PREFETCH_EN = 0;

    
    
    
    
    
    
    
    
        Flash0CtrlRegs.FRDCNTL.bit.RWAIT = 0x5;

    
    
    
    
    Flash0CtrlRegs.FRD_INTF_CTRL.bit.DATA_CACHE_EN = 1;
    Flash0CtrlRegs.FRD_INTF_CTRL.bit.PREFETCH_EN = 1;

    
    
    
    
    Flash0EccRegs.ECC_ENABLE.bit.ENABLE = 0xA;

    __edis();

    
    
    
    
    __asm(" RPT #7 || NOP");
}









void FlashOff(void)
{
    __eallow();

    
    
    
    Flash0CtrlRegs.FBFALLBACK.bit.BNKPWR0 = 0;

    
    
    
    Flash0CtrlRegs.FPAC1.bit.PMPPWR = 0;

    __edis();
}







void ServiceDog(void)
{
    __eallow();
    WdRegs.WDKEY.bit.WDKEY = 0x0055;
    WdRegs.WDKEY.bit.WDKEY = 0x00AA;
    __edis();
}




void DisableDog(void)
{
    volatile Uint16 temp;

    
    
    
    __eallow();
    temp = WdRegs.WDCR.all & 0x0007;
    WdRegs.WDCR.all = 0x0068 | temp;
    __edis();
}







void InitSysPll(Uint16 clock_source, Uint16 imult, Uint32 refdiv, Uint32 odiv,
                 Uint16 divsel, Uint32 dccbase)
{
    Uint32 timeout,temp_syspllmult, pllLockStatus;
    _Bool status;

    if(((clock_source & 0x3) == ClkCfgRegs.CLKSRCCTL1.bit.OSCCLKSRCSEL)    &&
       (((clock_source & 0x4) >> 2) == ClkCfgRegs.XTALCR.bit.SE)           &&
       (imult  == ClkCfgRegs.SYSPLLMULT.bit.IMULT)           &&
       (refdiv  == ClkCfgRegs.SYSPLLMULT.bit.REFDIV)          &&
       (odiv == ClkCfgRegs.SYSPLLMULT.bit.ODIV)               &&
       (divsel == ClkCfgRegs.SYSCLKDIVSEL.bit.PLLSYSCLKDIV))
    {
        
        
        
        return;
    }

    __eallow();

    
    
    
    
    if((imult != ClkCfgRegs.SYSPLLMULT.bit.IMULT) ||
       (refdiv != ClkCfgRegs.SYSPLLMULT.bit.REFDIV)||
       (odiv != ClkCfgRegs.SYSPLLMULT.bit.ODIV) ||
       (1U != ClkCfgRegs.SYSPLLCTL1.bit.PLLEN))
    {
        
        
        
        ClkCfgRegs.SYSPLLCTL1.bit.PLLCLKEN = 0;

        
        
        
        asm(" RPT #120 || NOP");

        
        
        
        temp_syspllmult = ((refdiv << 24U) | (odiv << 16U)| imult);

        
        
        
        ClkCfgRegs.SYSPLLCTL1.bit.PLLEN = 0;
        __edis();

        
        
        
        asm(" RPT #66 || NOP");

        if(((clock_source & 0x3) != ClkCfgRegs.CLKSRCCTL1.bit.OSCCLKSRCSEL) ||
          (((clock_source & 0x4) >> 2) != ClkCfgRegs.XTALCR.bit.SE))
        {
            switch (clock_source)
            {
                case 2:
                    SysIntOsc1Sel();
                    break;

                case 0:
                    SysIntOsc2Sel();
                    break;

                case 1:
                    SysXtalOscSel();
                    break;

                case 5:
                    SysXtalOscSESel();
                    break;
            }
        }

        
        
        
        asm(" RPT #60 || NOP");

        __eallow();

        
        
        
        ClkCfgRegs.SYSCLKDIVSEL.bit.PLLSYSCLKDIV = 0;

        
        
        
        ClkCfgRegs.SYSPLLMULT.all = temp_syspllmult;

        
        
        
        ClkCfgRegs.SYSPLLCTL1.bit.PLLEN = 1;

        
        
        
        timeout = (1024U * (refdiv + 1U));
        pllLockStatus = ClkCfgRegs.SYSPLLSTS.bit.LOCKS;

        
        
        
        while((pllLockStatus != 1) && (timeout != 0U))
        {
            pllLockStatus = ClkCfgRegs.SYSPLLSTS.bit.LOCKS;
            timeout--;
        }

        __edis();

        
        
        
        status = IsPLLValid(dccbase, clock_source, 0,
                            imult, odiv , refdiv);

    }
    else
    {
        
        
        
        
        status = 1;
    }

    if(status)
    {
        __eallow();
        
        
        
        if(divsel != 63)
        {
            ClkCfgRegs.SYSCLKDIVSEL.bit.PLLSYSCLKDIV = divsel + 1;
        }
        else
        {
            ClkCfgRegs.SYSCLKDIVSEL.bit.PLLSYSCLKDIV = divsel;
        }

        
        
        
        ClkCfgRegs.SYSPLLCTL1.bit.PLLCLKEN = 1;

        
        
        
        asm(" RPT #100 || NOP");

        
        
        
        ClkCfgRegs.SYSCLKDIVSEL.bit.PLLSYSCLKDIV = divsel;
        __edis();
    }
    else
        __asm(" ESTOP0"); 

}




void SysIntOsc1Sel(void)
{
    __eallow();
    ClkCfgRegs.CLKSRCCTL1.bit.OSCCLKSRCSEL = 2;     
    __edis();
}




void SysIntOsc2Sel(void)
{
    __eallow();
    ClkCfgRegs.CLKSRCCTL1.bit.OSCCLKSRCSEL = 0;     
    __edis();
}





static void
PollX1Counter(void)
{
    Uint16 loopCount = 0;

    
    
    
    
    
    F28x_usDelay(2000);

    
    
    
    do
    {
        
        
        
        while(ClkCfgRegs.X1CNT.all > 0x1FF)
        {
            ClkCfgRegs.X1CNT.bit.CLR = 1;
            ClkCfgRegs.X1CNT.bit.CLR = 0;
        }

        
        
        
        while(ClkCfgRegs.X1CNT.all != 0x7FFU)
        {
            ;
        }

        
        
        
        loopCount++;
    }while(loopCount < 4);
}


void SysXtalOscSel(void)
{
    __eallow();
    ClkCfgRegs.XTALCR.bit.OSCOFF = 0;     
    ClkCfgRegs.XTALCR.bit.SE = 0;         
    __edis();

    
    
    
    PollX1Counter();

    
    
    
    __eallow();
    ClkCfgRegs.CLKSRCCTL1.bit.OSCCLKSRCSEL = 1;
    __edis();

    
    
    
    
    while(ClkCfgRegs.MCDCR.bit.MCLKSTS != 0)
    {
        __eallow();
        ClkCfgRegs.MCDCR.bit.MCLKCLR = 1;
        __edis();

        
        
        
        PollX1Counter();

        
        
        
        __eallow();
        ClkCfgRegs.CLKSRCCTL1.bit.OSCCLKSRCSEL = 1;
        __edis();
    }
}






void
SysXtalOscSESel (void)
{
    __eallow();
    ClkCfgRegs.XTALCR.bit.OSCOFF = 0;     
    ClkCfgRegs.XTALCR.bit.SE = 1;         
    __edis();

    
    
    
    PollX1Counter();

    
    
    
    __eallow();
    ClkCfgRegs.CLKSRCCTL1.bit.OSCCLKSRCSEL = 1;
    __edis();

    
    
    
    
    if(ClkCfgRegs.MCDCR.bit.MCLKSTS != 0)
    {
        __asm(" ESTOP0");
    }
}




void
IDLE()
{
    __eallow();
    CpuSysRegs.LPMCR.bit.LPM = 0x0;
    __edis();
    asm(" IDLE");
}




void
HALT()
{
    __eallow();
    CpuSysRegs.LPMCR.bit.LPM = 0x2;
    ClkCfgRegs.SYSPLLCTL1.bit.PLLCLKEN = 0;
    ClkCfgRegs.SYSPLLCTL1.bit.PLLEN = 0;
    __edis();
    asm(" IDLE");
}






_Bool
IsPLLValid(Uint32 base, Uint16 oscSource, Uint16 pllclk, Uint16 imult,
           Uint16 odiv, Uint16 refdiv)
{
    float fclk1_0ratio;
    volatile struct DCC_REGS *DccRegs;

    __eallow();

    
    
    
    
    if(base == 0)
    {
            DccRegs = &Dcc0Regs;
            CpuSysRegs.PCLKCR21.bit.DCC0 = 1;
    }
    else if(base == 1)
    {
            DccRegs = &Dcc1Regs;
            CpuSysRegs.PCLKCR21.bit.DCC1 = 1;
    }
    else
        __asm(" ESTOP0"); 

    
    
    
    DccRegs->DCCSTATUS.bit.ERR = 1;
    DccRegs->DCCSTATUS.bit.DONE = 1;

    
    
    
    DccRegs->DCCGCTRL.bit.DCCENA = 0x5;

    
    
    
    DccRegs->DCCGCTRL.bit.ERRENA = 0x5;

    
    
    
    DccRegs->DCCGCTRL.bit.DONEENA = 0x5;

    
    
    
    
    
    switch(pllclk)
    {
        case 0:
            DccRegs->DCCCLKSRC1.all = 0xA000; 
            break;

        default:
            
            
            
            break;
    }

    
    
    
    
    
    
    switch(oscSource)
    {
        case 2:
            DccRegs->DCCCLKSRC0.all = 0xA001; 
            break;

        case 0:
            DccRegs->DCCCLKSRC0.all = 0xA002; 
            break;

        case 1:
        case 5:
            DccRegs->DCCCLKSRC0.all = 0xA000; 
            break;

        default:
            
            
            
            break;
    }

    
    
    
    fclk1_0ratio = (float)imult / ((odiv + 1U) * (refdiv + 1));

    
    
    
    
    ComputeCntrSeedValue(base, fclk1_0ratio, 1);

    
    
    
    DccRegs->DCCGCTRL.bit.SINGLESHOT = 0xA;

    
    
    
    DccRegs->DCCGCTRL.bit.DCCENA = 0xA;
    __edis();

    
    
    
    while((DccRegs->DCCSTATUS.all & 3) == 0)
    {
    }

    
    
    
    return((DccRegs->DCCSTATUS.all & 3) == 2);

}







void ComputeCntrSeedValue(Uint32 base, float fclk1_0ratio, Uint32 tolerance)
{
    Uint32 window, dccCounterSeed0, dccValidSeed0, dccCounterSeed1, total_error;
    volatile struct DCC_REGS *DccRegs;

    if(fclk1_0ratio >= 1U)
     {
        
        
        
        
        window = (12U * 100U) / tolerance;
        dccCounterSeed0 = window - 12U;
        dccValidSeed0 = 24U;

     }
    else
     {
        total_error = (((Uint32)2U / fclk1_0ratio) + (Uint32)10U);

        window = ((total_error * 100U)/ tolerance);

        
        
        
        dccCounterSeed0 = window - total_error;
        dccValidSeed0 = (Uint32)2U * total_error;

     }

    
    
    
    dccCounterSeed1 = window * fclk1_0ratio;

    
    
    
    if(base == 0)
        DccRegs = &Dcc0Regs;
    else if(base == 1)
        DccRegs = &Dcc1Regs;
    else
        __asm(" ESTOP0"); 

    
    
    
    DccRegs->DCCCNTSEED0.bit.COUNTSEED0 = dccCounterSeed0; 
    DccRegs->DCCVALIDSEED0.bit.VALIDSEED = dccValidSeed0;  
    DccRegs->DCCCNTSEED1.bit.COUNTSEED1 = dccCounterSeed1; 
}





