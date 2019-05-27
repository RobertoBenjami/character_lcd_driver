#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-Midicnt_dspic30f4013_XC16.mk)" "nbproject/Makefile-local-Midicnt_dspic30f4013_XC16.mk"
include nbproject/Makefile-local-Midicnt_dspic30f4013_XC16.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=Midicnt_dspic30f4013_XC16
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/demo-ora.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/demo-ora.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../charlcd.c ../demo-ora-c30.c ../config_dspic30f4013_xc16.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1472/charlcd.o ${OBJECTDIR}/_ext/1472/demo-ora-c30.o ${OBJECTDIR}/_ext/1472/config_dspic30f4013_xc16.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1472/charlcd.o.d ${OBJECTDIR}/_ext/1472/demo-ora-c30.o.d ${OBJECTDIR}/_ext/1472/config_dspic30f4013_xc16.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1472/charlcd.o ${OBJECTDIR}/_ext/1472/demo-ora-c30.o ${OBJECTDIR}/_ext/1472/config_dspic30f4013_xc16.o

# Source Files
SOURCEFILES=../charlcd.c ../demo-ora-c30.c ../config_dspic30f4013_xc16.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-Midicnt_dspic30f4013_XC16.mk dist/${CND_CONF}/${IMAGE_TYPE}/demo-ora.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=30F4013
MP_LINKER_FILE_OPTION=,--script=p30F4013.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/charlcd.o: ../charlcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/charlcd.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/charlcd.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../charlcd.c  -o ${OBJECTDIR}/_ext/1472/charlcd.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/charlcd.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1    -omf=elf -DXPRJ_Midicnt_dspic30f4013_XC16=$(CND_CONF)    $(COMPARISON_BUILD)  -O3 -I"D:/Programok/Microchip/xc16/v1.22/include" -DHARDWARE_MIDICNT_DSPICBOARD -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/charlcd.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/demo-ora-c30.o: ../demo-ora-c30.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/demo-ora-c30.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/demo-ora-c30.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../demo-ora-c30.c  -o ${OBJECTDIR}/_ext/1472/demo-ora-c30.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/demo-ora-c30.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1    -omf=elf -DXPRJ_Midicnt_dspic30f4013_XC16=$(CND_CONF)    $(COMPARISON_BUILD)  -O3 -I"D:/Programok/Microchip/xc16/v1.22/include" -DHARDWARE_MIDICNT_DSPICBOARD -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/demo-ora-c30.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/config_dspic30f4013_xc16.o: ../config_dspic30f4013_xc16.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/config_dspic30f4013_xc16.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/config_dspic30f4013_xc16.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../config_dspic30f4013_xc16.c  -o ${OBJECTDIR}/_ext/1472/config_dspic30f4013_xc16.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/config_dspic30f4013_xc16.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1    -omf=elf -DXPRJ_Midicnt_dspic30f4013_XC16=$(CND_CONF)    $(COMPARISON_BUILD)  -O3 -I"D:/Programok/Microchip/xc16/v1.22/include" -DHARDWARE_MIDICNT_DSPICBOARD -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/config_dspic30f4013_xc16.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/_ext/1472/charlcd.o: ../charlcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/charlcd.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/charlcd.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../charlcd.c  -o ${OBJECTDIR}/_ext/1472/charlcd.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/charlcd.o.d"        -g -omf=elf -DXPRJ_Midicnt_dspic30f4013_XC16=$(CND_CONF)    $(COMPARISON_BUILD)  -O3 -I"D:/Programok/Microchip/xc16/v1.22/include" -DHARDWARE_MIDICNT_DSPICBOARD -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/charlcd.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/demo-ora-c30.o: ../demo-ora-c30.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/demo-ora-c30.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/demo-ora-c30.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../demo-ora-c30.c  -o ${OBJECTDIR}/_ext/1472/demo-ora-c30.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/demo-ora-c30.o.d"        -g -omf=elf -DXPRJ_Midicnt_dspic30f4013_XC16=$(CND_CONF)    $(COMPARISON_BUILD)  -O3 -I"D:/Programok/Microchip/xc16/v1.22/include" -DHARDWARE_MIDICNT_DSPICBOARD -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/demo-ora-c30.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/config_dspic30f4013_xc16.o: ../config_dspic30f4013_xc16.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/config_dspic30f4013_xc16.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/config_dspic30f4013_xc16.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../config_dspic30f4013_xc16.c  -o ${OBJECTDIR}/_ext/1472/config_dspic30f4013_xc16.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/config_dspic30f4013_xc16.o.d"        -g -omf=elf -DXPRJ_Midicnt_dspic30f4013_XC16=$(CND_CONF)    $(COMPARISON_BUILD)  -O3 -I"D:/Programok/Microchip/xc16/v1.22/include" -DHARDWARE_MIDICNT_DSPICBOARD -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/config_dspic30f4013_xc16.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/demo-ora.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/demo-ora.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -omf=elf -DXPRJ_Midicnt_dspic30f4013_XC16=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_SIMULATOR=1,$(MP_LINKER_FILE_OPTION),--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/demo-ora.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/demo-ora.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_Midicnt_dspic30f4013_XC16=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}\\xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/demo-ora.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf  
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Midicnt_dspic30f4013_XC16
	${RM} -r dist/Midicnt_dspic30f4013_XC16

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
