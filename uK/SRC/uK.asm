;********************************************
;* FILE NAME:  uK.asm
;* PURPOSE: uKernel assembly program
;* Author: John Qin
;* Date: 6/23/2003
;********************************************
 
;****************************************
;*         uK/HC908MR ASM               *
;****************************************

						XDEF Entry
						XDEF asm_main
						XDEF sys_initial											
						XDEF uKERNEL_InitTimer
   						XDEF uKERNEL_SystemInterrupt
					    XDEF uKERNEL_fnSavregsAndResched
					    XDEF uKERNEL_fnStartNewTask  			    
					    XDEF uKERNEL_fnResume
					    XDEF uKERNEL_fnStartTimer
					    XDEF uKERNEL_fnRestartTimer
					    XDEF PWMDRV_fnINIT_PWM


					    XREF uKERNEL_fnCheckTask  					    
					    XREF uKERNEL_fnSchedule    
					    XREF uKERNEL_fnKillSelf           
          			    XREF uKernelTaskSP
					    XREF currentTask 
					    XREF uKernelCurrentFunc  
	
				    			    				

						Include 'mr16_register.inc'
						

                                ; here set to PWM1 channel 
INTERRUPT_CYCLES equ $FF  	    ; OS timer interrupt cycles,0x4, 0x20 degug,0x4d for 2ms
PWMMOD_VAL		 equ $FF

DEFAULT_RAM 		    SECTION SHORT

temp_long  ds 4
temp_word  ds 2
temp_byte1 ds 1
temp_byte2 ds 1

array: equ $320
len:   equ $3c 

DEFAULT_ROM				SECTION




;**************************************************************
;* Main_Init - This is the point where code starts executing  *
;*             after a RESET.                                 *
;**************************************************************
Entry:
asm_main:
       rsp
       clra
       clrx
       cli                 ; Allow interrupts to happen



sys_initial:
       mov #$9C,CONFIG     ; EDGE=1, INDEP=1, LVI=1
       sta COPCTL
       rts
		


;**************************************************************
;* uKERNEL_InitTimer - Turns on timer A channel 0 for an Output      *
;*              Compare in approximately 2ms. The timer       *
;*              interrupt service routine continually sets    *
;*              the next interrupt to occur 2ms later.        *
;**************************************************************
uKERNEL_InitTimer:
       mov   #$36,TASC     ; Timer A - Cleared + Stopped.
                           ;   0x36 Clicks once every 64 BUS Cycles
                           ;   0x30 Clicks once every BUS Cycle
                           ;    77t Clicks ~ 2ms

       mov   #$0,TACH0H    ; Set Output Compare to happen 77T clicks
       mov   INTERRUPT_CYCLES,TACH0L   ; after we start the timer. (~2ms). The
                           ; timer interrupt will set OC for another ~2ms.

       mov   #$50,TASC0    ; Timer A Channel 0,0x54
                           ; Set for Output Compare operation.

       mov   #$06,TASC     ; Start the timer 0x06 or 0x00

       rts


;**************************************************************
;* uKERNEL_SystemInterrupt - OS Timer Interrupt Service Routine. *
;*             				                                 *
;**************************************************************       
uKERNEL_SystemInterrupt:
      
	   pshh
	   pshx
	   tsx   ;sp+1 
	   stx TACH2L ;store x to a reg
	   stx  uKernelTaskSP 
	
	   jsr   uKERNEL_fnCheckTask	;jump to sub, may schedule
	 
       pulx
       pulh
       
       pshh
       pshx	   
       lda TASC0
       and #$7f
       sta TASC0              ; Clear O.C. Flag
       ldhx TACH0H
       aix  #INTERRUPT_CYCLES  ; Setup another interrupt in ~2ms
       sthx TACH0H	   
       pulx
       pulh
	   sta COPCTL       
	   
	   rti       
;**************************************************************
;* uKERNEL_SavregsAndResched - OS Re-schedule.                   *
;*             				                                 *
;**************************************************************       	   
uKERNEL_fnSavregsAndResched:	   
 ; first create an interrupt stack frame
	   pshx		;push X onto stack
	   psha		;push A onto stack, interrupt frame is A
	   tpa		;transfer CC reg to A
	   psha		;push A onto stack, Defer() disables interrupt
	   jmp uKERNEL_fnSchedule   ;jump
	   
       rts       
;**************************************************************
;* uKERNEL_StartNewTask - OS Start a new task.                   *
;*             				                                 *
;**************************************************************       	          
uKERNEL_fnStartNewTask:  
;create an interrupt frame
		
	   rsp

	   ldx uKernelTaskSP+1
	   txs  	
	   lda uKernelCurrentFunc+1  	   
   	   psha 
	   lda uKernelCurrentFunc     	   	      	   
   	   psha 
   	   ldx uKERNEL_fnKillSelf	   	   
	   pshx		;push X onto stack
	   lda uKernelCurrentFunc
	   psha		;push A onto stack, interrupt frame is A
	   tpa		;transfer CC reg to A
	   psha		;push A onto stack

	   sta COPCTL 
	   
	   rti   


;**************************************************************
;* uKERNEL_Resume - OS resume a task.                      *
;*             				                                 *
;**************************************************************       	          	   
uKERNEL_fnResume:
	   
	   rsp
	   
	   ldx uKernelTaskSP
	   txs  ;x-1 
	   pulx  
	   pulh
       pshh
       pshx	   
       lda   TASC0
       and   #$7f
       sta   TASC0              
       ldhx  TACH0H
       aix   #INTERRUPT_CYCLES  ; Setup another interrupt in ~2ms
       sthx  TACH0H	 
       pulx  
       pulh
       
	   sta COPCTL   
	          	          
       rti
 
	
;**************************************************************
;* uKERNEL_StartTimer - OS start the OS timer.                       *
;*             				                                 *
;**************************************************************       	          	   
uKERNEL_fnStartTimer:
       mov   #$36,TASC     ; Timer A - Cleared + Stopped.
                           ;   0x36 Clicks once every 64 BUS Cycles
                           ;   0x30 Clicks once every BUS Cycle
                           ;    77t Clicks ~ 2ms
       mov   #$0,TACH0H    ; Set Output Compare to happen 77T clicks
       mov   #INTERRUPT_CYCLES,TACH0L   ; after we start the timer. (~2ms). The
                           ; timer interrupt will set OC for another ~2ms.
       mov   #$50,TASC0    ; Timer A Channel 0,#$54
                           ; Set for Output Compare operation, does not control any pin.
       mov   #$06,TASC     ; Start the timer, 0x6 or 0x0      

	   rts

;**************************************************************
;* uKERNEL_RestartTimer - OS restart the OS timer.            *
;*             				                                  *
;**************************************************************       	          	   	   
uKERNEL_fnRestartTimer:
       mov   #$36,TASC     ; Timer A - Cleared + Stopped.
                           ;   0x36 Clicks once every 64 BUS Cycles
                           ;   0x30 Clicks once every BUS Cycle
                           ;    77t Clicks ~ 2ms
       mov   #$0,TACH0H    ; Set Output Compare to happen 77T clicks
       mov   #INTERRUPT_CYCLES,TACH0L; after we start the timer. (~2ms). The
                           ; timer interrupt will set OC for another ~2ms.
       mov   #$50,TASC0    ; Timer A Channel 0, set bits, clear the interrupt flag
                           ; Set for Output Compare operation.0x54
       mov   #$06,TASC     ; Start the timer, 0x6 or 0x0     

	   rts
   	   
;**************************************************************
;* Init_PWMMC -   initialize PWM 6 channels    *
;**************************************************************
PWMDRV_fnINIT_PWM:
       ldhx  #PWMMOD_VAL     ; Load Counter Modulo Register with 256T
       sthx  PMODH
       ldhx  #$00            ; all PWM pins initially have 0% duty cycle
       sthx  PVAL1L          ; all PWM pins = 0
       sthx  PVAL1H          ; all PWM pins = 0       
       sthx  PVAL2H
       sthx  PVAL3H
       sthx  PVAL4H
       sthx  PVAL5H
       sthx  PVAL6H
       mov   #$3f,PWMOUT	  ; 0x3F, 6 independent
       mov   #$00,PCTL2       ; Reload every PWM cycle, fastest PWM frequency
       mov   #$03,PCTL1       ; no interrupt, load parameters, PWM on
       mov   #$00, DISMAP	  ; no pin disable when fault happens
       rts

	   

END
