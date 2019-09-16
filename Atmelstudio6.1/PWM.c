
009    

010 #include <avr/io.h>  

011 #include <util/delay.h>  

012    

013 void init(){  

014     PORTD = (1<<OC1A_BIT)|(1<<OC1B_BIT)|(1<<OC2_BIT);;  

015     DDRD = (1<<OC1A_BIT)|(1<<OC1B_BIT)|(1<<OC2_BIT);  

016    

017     //T1/C1 初始化  

018     TCCR1A = (1<<COM1A1)|(0<<COM1A0)|(1<<COM1B1)|(0<<COM1B0)|(0<<FOC1A)|(0<<FOC1B)|(0<<WGM11)|(1<<WGM10);  

019     /*  

020     * 非PWM状态  

021     *  

022     * COM1A1/COM1B1 COM1A0/COM1B0 操作  

023     * 0             0             普通I/O OC1A/OC1B不使能  

024     * 0             1             比较匹配OC1A/OC1B时候取反  

025     * 1             0             比较匹配OC1A/OC1B时候清零  

026     * 1             1             比较匹配OC1A/OC1B时候置位  

027     *  

028     * 快速PWM状态  

029     *  

030     * COM1A1/COM1B1 COM1A0/COM1BO 操作  

031     * 0             0             普通I/O OC1A/OC1B不使能  

032     * 0             1             WGM13:0 = 15 OC1A取反 OC1B不占用 WGM13:0 != 15 非PWM  

033     * 1             0             比较匹配OC1A/OC1B时候清零,OC1A/OC1B到TOP时候置位  

034     * 1             1             比较匹配OC1A/OC1B时候置位,OC1A/OC1B到TOP时候清零  

035     *  

036     * 比较输出,相位修正,相频修正PWM  

037     *  

038     * COM1A1/COM1B1 COM1A0/COM1BO 操作  

039     * 0             0             普通I/O OC1A/OC1B不使能  

040     * 0             1             WGM13:0 = 9(或14) OC1A取反 OC1B不占用 WGM13:0 != 15 非PWM  

041     * 1             0             上升沿比较匹配时候OC1A/OC1B清零,下降沿比较匹配时OC1A/OC1B置位  

042     * 1             1             上升沿比较匹配时候OC1A/OC1B置位,下降沿比较匹配时OC1A/OC1B清零  

043     *  

044     * FOC1A A通道强制输出比较  

045     * FOC1B B通道强制输出比较  

046     *  

047     * 上面两位的读取总是为0,他们的有效条件是COM1A1/COM1B1 和 COM1A0/COM1B0 均为0  

048     *   

049     * 波形发生模式(关系到TCCR1B)  

050     *   

051     * WGM13 WGM12 WGM11 WGM10  操作                   TOP    OCCR1x更新时刻 TOV1置位  

052     * 0     0     0     0      普通模式               0xFFFF 立即           MAX  

053     * 0     0     0     1      8位相位修正PWM         0x00FF TOP            BUTTOM  

054     * 0     0     1     0      9位相位修正PWM         0x01FF TOP            BOTTOM  

055     * 0     0     1     1      10位相位修正PWM        0x03FF TOP            BOTTOM  

056     * 0     1     0     0      CTC                    OCCR1A 立即           MAX  

057     * 0     1     0     1      8位快速修正PWM         0x00FF TOP            TOP  

058     * 0     1     1     0      9位快速修正PWM         0x01FF TOP            TOP  

059     * 0     1     1     1      10位快速修正PWM        0x03FF TOP            TOP  

060     * 1     0     0     0      相位与频率修正PWM      ICR1   BOTTOM         BOTTOM  

061     * 1     0     0     1      相位与频率修正PWM      OCR1A  BOTTOM         BOTTOM  

062     * 1     0     1     0      相位修正PWM            ICR1   TOP            BOTTOM  

063     * 1     0     1     1      相位修正PWM            OCR1A  TOP            BOTTOM  

064     * 1     1     0     0      CTC                    ICR1   立即           MAX  

065     * 1     1     1     0      快速PWM                ICR1   TOP            TOP  

066     * 1     1     1     1      快速PWM                OCR1A  TOP            TOP  

067     *  

068     * 别名:WGM12 = CTC1 WGM11 = PWM11 WGM10 = PWM10  

069     */ 

070     TCCR1B = (0<<ICNC1)|(0<<ICES1)|(0<<WGM13)|(1<<WGM12)|(0<<CS12)|(0<<CS11)|(1<<CS10);  

071     /*  

072     * ICNC1 滤波器,如果4个机器周期数据一样,就输入边沿检测器  

073     * ICES1 = 0 下降沿捕捉 ICES1 = 1 上升沿捕捉  

074     * WGM13/WGM12 波形发生模式,与TCCR1A有关  

075     *   

076     * 三个控制寄存器:CS12 CS11 CS10  

077     *   

078     * 0 0 0                   停止计时  

079     * 0 0 1                   无分频  

080     * 0 1 0                   8分频  

081     * 0 1 1                   64分频  

082     * 1 0 0                   256分频  

083     * 1 0 1                   1024分频  

084     * 1 1 0                   下降沿驱动  

085     * 1 1 1                   上升沿驱动  

086     */ 

087            

088     TCCR0 = (1<<CS12)|(0<<CS11)|(1<CS10);  

089     /*  

090     * 三个控制寄存器:CS12 CS11 CS10  

091     *   

092     * 0 0 0                   停止计时  

093     * 0 0 1                   无分频  

094     * 0 1 0                   8分频  

095     * 0 1 1                   64分频  

096     * 1 0 0                   256分频  

097     * 1 0 1                   1024分频  

098     * 1 1 0                   下降沿驱动  

099     * 1 1 1                   上升沿驱动  

100     */ 

101     OCR1A=(int)(255/3); //输出1/3占空比的PWM        

102     OCR1B=(int)(255/2); //输出1/2占空比的PWM  

103     /*  

104     * OCR1A = OCR1AH + OCR1AL  

105     * OCR1B = OCR1BH + OCR1BH  

106     *  

107     * 输出比较寄存器  

108     */ 

109        

110     /*  

111     * ICR1 = ICR1H + ICR1L  

112     *  

113     * 输入捕捉寄存器  

114     */ 

115        

116     // T/C2 初始化  

117     TCCR2=(0<<FOC2)|(1<<WGM20)|(1<<COM21)|(1<<COM20)|(1<<WGM21)|(0<<CS22)|(0<<CS21)|(1<<CS20);   

118     /*  

119     *  

120     * FOC2 强制输出比较,非PWM下有效  

121     *  

122     * WGM21 WGM20 T/C工作模式 TOP  OCR2更新时间 OCR2置位时间  

123     * 0     0     普通        0xFF 立即更新     MAX  

124     * 0     1     相位修正PWM 0xFF TOP          BOTTOM  

125     * 1     0     CTC         OCR2 立即更新     MAX  

126     * 1     1     快速PWM     0xFF TOP          MAX  

127     *  

128     * COM21 COM20 非PWM作用               快速PWM作用                            相位修正PWM作用  

129     * 0     1     比较匹配发生时,OC0取反  无效  

130     * 1     0     比较匹配发生时,OC0清零  比较匹配时OC0清零,计算到TOP时,OC0置位. 在上升沿比较匹配,则清零OC2,下降沿比较匹配,则置位OC2  

131     * 1     1     比较匹配发生时,OC0置位  比较匹配时OC0置位,计算到TOP时,OC0清零. 在上升沿比较匹配,则置位OC2,下降沿比较匹配,则清零OC2  

132     *  

133     * 三个控制寄存器:CS12 CS11 CS10  

134     *  

135     * 0 0 0                   停止计时  

136     * 0 0 1                   无分频  

137     * 0 1 0                   8分频  

138     * 0 1 1                   32分频  

139     * 1 0 0                   64分频  

140     * 1 0 1                   128分频  

141     * 1 1 0                   256分频  

142     * 1 1 1                   1024分频  

143     */ 

144     OCR2=(int)(255/4);  

145 }  

146    

147 int main(void)  

148 {  

149     int i;  

150     init();  

151     while(1)  

152     {  

153            

154         for (i=0;i<100;i++)  

155         {  

156             _delay_ms(10);  

157             OCR2 = i;  

158             OCR1A = i/2;  

159             OCR1B = i/3;  

160             if (i == 99)  

161             {  

162                 for (i = 100;i>0;i++)  

163                 {  

164                     _delay_ms(10);  

165                     OCR2 = i;  

166                     OCR1A = i/2;  

167                     OCR1A = i/3;  

168                 }  

169             }  

170         }  

171         //TODO:: Please write your application code   

172     }  

173 } 

