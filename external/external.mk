# Can include custom make logic and include packages .mk
include $(sort $(wildcard $(BR2_EXTERNAL_EMB22109_PATH)/package/*/*.mk))
