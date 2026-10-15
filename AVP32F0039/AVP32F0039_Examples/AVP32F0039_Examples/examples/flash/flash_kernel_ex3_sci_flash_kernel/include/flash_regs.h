//Flash and ECC register frame base addresses and offsets

#ifndef __FLASH_REGS_H
#define __FLASH_REGS_H

#if defined(C28x1) || defined(C28x2) || defined(_C28X)
//Flash and ECC register frame base addresses and offsets

//FMC register frame base address
#define FMC_REG_BASE          ((volatile uint32 *)0x5F800)

//ECC register frame base address
//#define ECC_REG_BASE          ((void *)0x5FB00)
#define ECC_REG_BASE          ((volatile uint32_t *)0x5FB00)
//Pump trim register in the analog system control frame
//#define ASYS_LPPUMPTRIM       (*(volatile uint32_t *)0x5D71c) // may be NA for Tenor as HDP


//FMC register offsets (from the Harmony spec)
#define FRDCNTL         0x0/2
#define FSPRD           0x2/2
#define FBBUSY          0x1c/2
#define FBAC            0x1e/2
#define FBFALLBACK      0x20/2
#define FBPRDY          0x22/2
#define FPAC1           0x24/2
#define FPAC2           0x26/2
#define FMAC            0x28/2
#define FMSTAT          0x2a/2
#define FLOCK           0x32/2
#define FRD_INTF_CTRL   0x180/2
//#define FRD_INTF_CTRL_LOCK  0x2FC/4
//#define FRD_INTF_CTRL   0x300/4

//Voltage register offsets (copied from the flash shell)
#define FVREADCT        0x080/4
#define FVHVCT1         0x084/4
#define FVHVCT2         0x088/4
#define FVHVCT3         0x08C/4
#define FVINCT          0x090/4
#define FVSLCT          0x094/4
#define FVWLCT          0x098/4
#define FSEQPMP         0x0A8/4

//Command registers (copied from the flash shell)
#define FBSTROBES       0x100/4
#define FPSTROBES       0x104/4
#define FBMODE          0x108/4
#define FADDR           0x110/4
#define FTCTRL          0x11C/4
#define FWPWRITE0       0x120/4
#define FWPWRITE1       0x124/4
#define FWPWRITE2       0x128/4
#define FWPWRITE3       0x12C/4
#define FWPWRITE4       0x130/4
#define FWPWRITE5       0x134/4
#define FWPWRITE6       0x138/4
#define FWPWRITE7       0x13C/4
#define FWPWRITE_ECC    0x140/4

//FSM registers (copied from the flash shell)
//c28x right (0x2??) ?? 2-nibbles divide by 2 to get mem window addr; ignore 0x2
#define FSM_GLBCTRL      0x200/4
#define FSM_COMMAND      0x20C/4
#define FSM_PE_OSU       0x210/4
#define FSM_VSTAT        0x214/4
#define FSM_PE_VSU       0x218/4
#define FSM_CMP_VSU      0x21C/4
#define FSM_EX_VAL       0x220/4
#define FSM_RD_H         0x224/4
#define FSM_P_OH         0x228/4
#define FSM_ERA_OH       0x22C/4
#define FSM_P_VH         0x234/4
#define FSM_PRG_PW       0x240/4
#define FSM_ERA_PW       0x244/4
#define FSM_PRG_PUL      0x268/4
#define FSM_ERA_PUL      0x26C/4
#define FSM_STEP_SIZE    0x270/4
#define FSM_STEP_HEIGHT  0x278/4
#define FSM_ST_MACHINE   0x27C/4
#define FSM_FLES         0x280/4
#define FSM_WR_ENA       0x288/4
#define FSM_ACC_PP       0x28C/4
#define FSM_ACC_EP       0x290/4
#define FSM_SECTOR       0x2A4/4
#define FSM_REV_ID       0x2A8/4
#define FSM_ERR_ADDR     0x2AC/4
#define FSM_PGM_MAXPUL   0x2B0/4
#define FSM_EXECUTE      0x2B4/4
#define FSM_SECTOR1      0x2C0/4

//Version info registers (from FMC spec)
#define FCFG_BANK        0x400/4
#define FCFG_WRAPPER     0x404/4
#define FCFG_BNK_TYPE    0x408/4
#define FCFG_B0_START    0x410/4
#define FCFG_B1_START    0x414/4
#define FCFG_B0_SSIZE0   0x430/4
#define FCFG_B0_SSIZE1   0x434/4
#define FCFG_B0_SSIZE2   0x438/4
#define FCFG_B0_SSIZE3   0x43C/4
#define FCFG_B1_SSIZE0   0x440/4
#define FCFG_B1_SSIZE1   0x444/4
#define FCFG_B1_SSIZE2   0x448/4
#define FCFG_B1_SSIZE3   0x44C/4

//ECC register offsets (from Harmony spec)
//These are converted to FMC offsets to make testing easier
/**///#define ECC_ENABLE            (0x300/2 + 0x0/2)
#define ECC_ENABLE            0x0/2
#define SINGLE_ERR_ADDR_LOW   0x2/2
#define SINGLE_ERR_ADDR_HIGH  0x4/2
#define UNC_ERR_ADDR_LOW      0x6/2
#define UNC_ERR_ADDR_HIGH     0x8/2
#define ERR_STATUS            0xa/2
#define ERR_POS               0xc/2
#define ERR_STATUS_CLR        0xe/2
#define ERR_CNT               0x10/2
#define ERR_THRESHOLD         0x12/2
#define ERR_INTFLG            0x14/2
#define ERR_INTCLR            0x16/2
#define FDATAH_TEST           0x18/2
#define FDATAL_TEST           0x1a/2
#define FADDR_TEST            0x1c/2
#define FECC_TEST             0x1e/2
#define FECC_CTRL             0x20/2
#define FOUTH_TEST            0x22/2
#define FOUTL_TEST            0x24/2
#define FECC_STATUS           0x26/2

#endif // define C28x1

#endif //__FLASH_REGS_H
