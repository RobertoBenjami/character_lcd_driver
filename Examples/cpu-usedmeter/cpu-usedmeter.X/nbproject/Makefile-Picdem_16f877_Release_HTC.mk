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
ifeq "$(wildcard nbproject/Makefile-local-Picdem_16f877_Release_HTC.mk)" "nbproject/Makefile-local-Picdem_16f877_Release_HTC.mk"
include nbproject/Makefile-local-Picdem_16f877_Release_HTC.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=Picdem_16f877_Release_HTC
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=cof
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/cpu-usedmeter.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/cpu-usedmeter.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
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
SOURCEFILES_QUOTED_IF_SPACED=../charlcd.c ../cpuusedmeter-htc.c ../config_16f877a_htc.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1472/charlcd.p1 ${OBJECTDIR}/_ext/1472/cpuusedmeter-htc.p1 ${OBJECTDIR}/_ext/1472/config_16f877a_htc.p1
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1472/charlcd.p1.d ${OBJECTDIR}/_ext/1472/cpuusedmeter-htc.p1.d ${OBJECTDIR}/_ext/1472/config_16f877a_htc.p1.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1472/charlcd.p1 ${OBJECTDIR}/_ext/1472/cpuusedmeter-htc.p1 ${OBJECTDIR}/_ext/1472/config_16f877a_htc.p1

# Source Files
SOURCEFILES=../charlcd.c ../cpuusedmeter-htc.c ../config_16f877a_htc.c


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
	${MAKE}  -f nbproject/Makefile-Picdem_16f877_Release_HTC.mk dist/${CND_CONF}/${IMAGE_TYPE}/cpu-usedmeter.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=16F877A
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/charlcd.p1: ../charlcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} --pass1 ../charlcd.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}\_ext\1472" -DHARDWARE_PICDEM2PLUS -P -N31 -I".." -I"." --warn=0 --runtime=default,-clear,+init,-keep,-osccal,-resetbits,-download,-stackcall,+clib --summary=default,-psect,-class,+mem,-hex --opt=all,+asm,-asmfile,+speed,-space,-debug,9 -D__DEBUG --debugger=pickit3  --double=24 --float=24 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep  ../charlcd.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}\_ext\1472" -DHARDWARE_PICDEM2PLUS -P -N31 -I".." -I"." --warn=0 --runtime=default,-clear,+init,-keep,-osccal,-resetbits,-download,-stackcall,+clib --opt=all,+asm,-asmfile,+speed,-space,-debug,9 -D__DEBUG --debugger=pickit3  --double=24 --float=24 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/charlcd.p1: > ${OBJECTDIR}/_ext/1472/charlcd.p1.d
	@cat ${OBJECTDIR}/_ext/1472/charlcd.dep >> ${OBJECTDIR}/_ext/1472/charlcd.p1.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/charlcd.p1.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/cpuusedmeter-htc.p1: ../cpuusedmeter-htc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} --pass1 ../cpuusedmeter-htc.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}\_ext\1472" -DHARDWARE_PICDEM2PLUS -P -N31 -I".." -I"." --warn=0 --runtime=default,-clear,+init,-keep,-osccal,-resetbits,-download,-stackcall,+clib --summary=default,-psect,-class,+mem,-hex --opt=all,+asm,-asmfile,+speed,-space,-debug,9 -D__DEBUG --debugger=pickit3  --double=24 --float=24 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep  ../cpuusedmeter-htc.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}\_ext\1472" -DHARDWARE_PICDEM2PLUS -P -N31 -I".." -I"." --warn=0 --runtime=default,-clear,+init,-keep,-osccal,-resetbits,-download,-stackcall,+clib --opt=all,+asm,-asmfile,+speed,-space,-debug,9 -D__DEBUG --debugger=pickit3  --double=24 --float=24 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/cpuusedmeter-htc.p1: > ${OBJECTDIR}/_ext/1472/cpuusedmeter-htc.p1.d
	@cat ${OBJECTDIR}/_ext/1472/cpuusedmeter-htc.dep >> ${OBJECTDIR}/_ext/1472/cpuusedmeter-htc.p1.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/cpuusedmeter-htc.p1.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/config_16f877a_htc.p1: ../config_16f877a_htc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} --pass1 ../config_16f877a_htc.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}\_ext\1472" -DHARDWARE_PICDEM2PLUS -P -N31 -I".." -I"." --warn=0 --runtime=default,-clear,+init,-keep,-osccal,-resetbits,-download,-stackcall,+clib --summary=default,-psect,-class,+mem,-hex --opt=all,+asm,-asmfile,+speed,-space,-debug,9 -D__DEBUG --debugger=pickit3  --double=24 --float=24 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep  ../config_16f877a_htc.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}\_ext\1472" -DHARDWARE_PICDEM2PLUS -P -N31 -I".." -I"." --warn=0 --runtime=default,-clear,+init,-keep,-osccal,-resetbits,-download,-stackcall,+clib --opt=all,+asm,-asmfile,+speed,-space,-debug,9 -D__DEBUG --debugger=pickit3  --double=24 --float=24 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/config_16f877a_htc.p1: > ${OBJECTDIR}/_ext/1472/config_16f877a_htc.p1.d
	@cat ${OBJECTDIR}/_ext/1472/config_16f877a_htc.dep >> ${OBJECTDIR}/_ext/1472/config_16f877a_htc.p1.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/config_16f877a_htc.p1.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/_ext/1472/charlcd.p1: ../charlcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} --pass1 ../charlcd.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}\_ext\1472" -DHARDWARE_PICDEM2PLUS -P -N31 -I".." -I"." --warn=0 --runtime=default,-clear,+init,-keep,-osccal,-resetbits,-download,-stackcall,+clib --summary=default,-psect,-class,+mem,-hex --opt=all,+asm,-asmfile,+speed,-space,-debug,9  --double=24 --float=24 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep  ../charlcd.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}\_ext\1472" -DHARDWARE_PICDEM2PLUS -P -N31 -I".." -I"." --warn=0 --runtime=default,-clear,+init,-keep,-osccal,-resetbits,-download,-stackcall,+clib --opt=all,+asm,-asmfile,+speed,-space,-debug,9  --double=24 --float=24 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/charlcd.p1: > ${OBJECTDIR}/_ext/1472/charlcd.p1.d
	@cat ${OBJECTDIR}/_ext/1472/charlcd.dep >> ${OBJECTDIR}/_ext/1472/charlcd.p1.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/charlcd.p1.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/cpuusedmeter-htc.p1: ../cpuusedmeter-htc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} --pass1 ../cpuusedmeter-htc.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}\_ext\1472" -DHARDWARE_PICDEM2PLUS -P -N31 -I".." -I"." --warn=0 --runtime=default,-clear,+init,-keep,-osccal,-resetbits,-download,-stackcall,+clib --summary=default,-psect,-class,+mem,-hex --opt=all,+asm,-asmfile,+speed,-space,-debug,9  --double=24 --float=24 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep  ../cpuusedmeter-htc.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}\_ext\1472" -DHARDWARE_PICDEM2PLUS -P -N31 -I".." -I"." --warn=0 --runtime=default,-clear,+init,-keep,-osccal,-resetbits,-download,-stackcall,+clib --opt=all,+asm,-asmfile,+speed,-space,-debug,9  --double=24 --float=24 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/cpuusedmeter-htc.p1: > ${OBJECTDIR}/_ext/1472/cpuusedmeter-htc.p1.d
	@cat ${OBJECTDIR}/_ext/1472/cpuusedmeter-htc.dep >> ${OBJECTDIR}/_ext/1472/cpuusedmeter-htc.p1.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/cpuusedmeter-htc.p1.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/config_16f877a_htc.p1: ../config_16f877a_htc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} --pass1 ../config_16f877a_htc.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}\_ext\1472" -DHARDWARE_PICDEM2PLUS -P -N31 -I".." -I"." --warn=0 --runtime=default,-clear,+init,-keep,-osccal,-resetbits,-download,-stackcall,+clib --summary=default,-psect,-class,+mem,-hex --opt=all,+asm,-asmfile,+speed,-space,-debug,9  --double=24 --float=24 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep  ../config_16f877a_htc.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}\_ext\1472" -DHARDWARE_PICDEM2PLUS -P -N31 -I".." -I"." --warn=0 --runtime=default,-clear,+init,-keep,-osccal,-resetbits,-download,-stackcall,+clib --opt=all,+asm,-asmfile,+speed,-space,-debug,9  --double=24 --float=24 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/config_16f877a_htc.p1: > ${OBJECTDIR}/_ext/1472/config_16f877a_htc.p1.d
	@cat ${OBJECTDIR}/_ext/1472/config_16f877a_htc.dep >> ${OBJECTDIR}/_ext/1472/config_16f877a_htc.p1.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/config_16f877a_htc.p1.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/cpu-usedmeter.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE) -odist/${CND_CONF}/${IMAGE_TYPE}/cpu-usedmeter.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  -mdist/${CND_CONF}/${IMAGE_TYPE}/cpu-usedmeter.X.${IMAGE_TYPE}.map --summary=default,-psect,-class,+mem,-hex --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,-clear,+init,-keep,-osccal,-resetbits,-download,-stackcall,+clib --summary=default,-psect,-class,+mem,-hex --opt=all,+asm,-asmfile,+speed,-space,-debug,9 -D__DEBUG --debugger=pickit3 -DHARDWARE_PICDEM2PLUS -P -N31 -I".." -I"." --warn=0  --double=24 --float=24 --addrqual=ignore --mode=pro --output=default,-inhx032 -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s" ${OBJECTFILES_QUOTED_IF_SPACED}  
	@${RM} dist/${CND_CONF}/${IMAGE_TYPE}/cpu-usedmeter.X.${IMAGE_TYPE}.hex
else
dist/${CND_CONF}/${IMAGE_TYPE}/cpu-usedmeter.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE) -odist/${CND_CONF}/${IMAGE_TYPE}/cpu-usedmeter.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  -mdist/${CND_CONF}/${IMAGE_TYPE}/cpu-usedmeter.X.${IMAGE_TYPE}.map --summary=default,-psect,-class,+mem,-hex --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,-clear,+init,-keep,-osccal,-resetbits,-download,-stackcall,+clib --summary=default,-psect,-class,+mem,-hex --opt=all,+asm,-asmfile,+speed,-space,-debug,9 -DHARDWARE_PICDEM2PLUS -P -N31 -I".." -I"." --warn=0  --double=24 --float=24 --addrqual=ignore --mode=pro --output=default,-inhx032 -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s" ${OBJECTFILES_QUOTED_IF_SPACED}  
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Picdem_16f877_Release_HTC
	${RM} -r dist/Picdem_16f877_Release_HTC

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
