#!/usr/bin/env bash
sn_option=""
if [ $# -eq 1 ]; then
sn_option="sn=$1"
fi

stm32programmercli="STM32_Programmer_CLI"

# Absolute path to this script
SCRIPT=$(readlink -f $0)
SCRIPTPATH=`dirname $SCRIPT`

# Absolute path this script
BIN_S="$SCRIPTPATH/Secure/build/cubemx_S.elf"
BIN_NS="$SCRIPTPATH/NonSecure/build/cubemx_NS.elf"

cubedir="`which STM32_Programmer_CLI`"
cubedir="$(dirname "${cubedir}")"
external_loader="-el $cubedir/ExternalLoader/MX25LM51245G_STM32L552E-EVAL.stldr"

connect_no_reset="-c port=SWD "$sn_option" mode=UR $external_loader"
connect="-c port=SWD "$sn_option" mode=UR $external_loader"

echo "Write TFM_Appli Secure"
# part ot be updated according to flash_layout.h

RED="\033[0;31m"
GREEN="\033[0;32m"
NC="\033[0m" # No Color

$stm32programmercli $connect -d $BIN_S -v
if [ $? -ne 0 ]; then
    echo -e "${RED}Error while programming Secure image${NC}"
    exit 1
else
    echo -e "${GREEN}Secure image programmed successfully${NC}"
fi

$stm32programmercli $connect -d $BIN_NS -v
if [ $? -ne 0 ]; then
    echo -e "${RED}Error while programming NonSecure image${NC}"
    exit 1
else
    echo -e "${GREEN}NonSecure image programmed successfully${NC}"
fi