TLSR8258

泰凌微 TLSR8258 PWM:

[pwm]: https://blog.csdn.net/weixin_45056123/article/details/133687917?utm_medium=distribute.pc_relevant.none-task-blog-2~default~baidujs_baidulandingword~default-0-133687917-blog-133383900.235^v38^pc_relevant_anti_t3&amp;spm=1001.2101.3001.4242.1&amp;utm_relevant_index=3



```c
gpio_set_func(LED_G, AS_PWM0_N);//设置LED_G为pwm0
drv_pwm_n_invert(PWM_G_CHANNEL);
```



sampleLight_colorCtrlCb()

ZCL COLOR CONTROL命令的处理程序。此函数将首先设置“颜色控制”属性。
