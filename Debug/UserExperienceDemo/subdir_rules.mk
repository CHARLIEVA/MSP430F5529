################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
UserExperienceDemo/mouse.obj: ../UserExperienceDemo/mouse.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_4.4.3/bin/cl430" -vmspx --abi=coffabi --data_model=restricted -O0 --use_hw_mpy=F5 --include_path="C:/ti/ccsv6/ccs_base/msp430/include" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_4.4.3/include" --include_path="C:/Users/93425/Desktop/workspace_v6/Copy of Wendu_1/F5xx_F6xx_Core_Lib" --include_path="C:/Users/93425/Desktop/workspace_v6/Copy of Wendu_1/MSP-EXP430F5529_HAL" --include_path="C:/Users/93425/Desktop/workspace_v6/Copy of Wendu_1/FatFs" --include_path="C:/Users/93425/Desktop/workspace_v6/Copy of Wendu_1/CTS" --include_path="C:/Users/93425/Desktop/workspace_v6/Copy of Wendu_1/USB" --include_path="C:/Users/93425/Desktop/workspace_v6/Copy of Wendu_1/USB/USB_API/USB_CDC_API" --include_path="C:/Users/93425/Desktop/workspace_v6/Copy of Wendu_1/USB/USB_API/USB_Common" --include_path="C:/Users/93425/Desktop/workspace_v6/Copy of Wendu_1/USB/USB_API/USB_HID_API" --include_path="C:/Users/93425/Desktop/workspace_v6/Copy of Wendu_1/USB/USB_User" --include_path="C:/Users/93425/Desktop/workspace_v6/Copy of Wendu_1/USB/USB_API" --include_path="C:/Users/93425/Desktop/workspace_v6/Copy of Wendu_1/USB/USB_config" --include_path="C:/Users/93425/Desktop/workspace_v6/Copy of Wendu_1/UserExperienceDemo" --include_path="C:/Users/93425/Desktop/workspace_v6/Copy of Wendu_1/UserExperienceDemo/Puzzle" -g --gcc --define=__MSP430F5529__ --diag_warning=225 --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --large_memory_model --printf_support=minimal --preproc_with_compile --preproc_dependency="UserExperienceDemo/mouse.pp" --obj_directory="UserExperienceDemo" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


