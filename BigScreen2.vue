<template>
    <dv-full-screen-container >
        <div id="bg_div">
            <div id="headPart" style="transform: scale(0.5,0.5)">
                <div id="headPartWord">
                    <h1 id="headPartH1" class="animate__animated animate__pulse animate__delay-1s" style="font-size: 35px;">嵌入式-系统设计大赛</h1>
                </div>
            </div>

            <div id="timeAndDay">
                <div id="timeDiv" ref="timeDivValue">{{domTimeDiv}}</div>
                <div id="dayDiv"  ref="dayDivValue">{{domDayDiv}}</div>
            </div> 
            <div id="CNweather">
                <div class="weather-info" v-if="weather">
                    <div class="Leftinfo-item">
                        <p class="label">城市:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{{ weather.name }}</p>
                    </div>
                    <div class="Rightinfo-item">
                        <p class="label">天气:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{{ weather.weather[0].description }}</p>
                    </div>
                </div>
                <div v-else>
                    <p>加载中...</p>
                </div>
            </div>

            <div id = "leftDiv">
                <div id="leftDivLine1">
                    <div id="leftDivLine1LabelDiv">
                        <p>报警按钮是否被按下</p>
                    </div>
                </div>
                <div id="leftDivLine2">
                    <p :style="{ color: bjlablevalue.value === '是' ? 'red' : 'aqua', fontSize: '45px'}">
                        {{ bjlablevalue.value }}
                    </p>
                    <!-- <div class="leftDivLine2Div">
                        <p class="leftDivLine2DivUpLabel">累加数据1</p>
                        <p class="leftDivLine2DivDownLabel">全年订单量</p>
                    </div>
                    <div class="leftDivLine2Div">
                        <p class="leftDivLine2DivUpLabel">累加数据2</p>
                        <p class="leftDivLine2DivDownLabel">当月订单量</p>
                    </div> -->
                </div>

                <div id="leftDivLine3">
                    <div id="leftDivLine3UpDiv">   <!--图片-->
                        <div id="leftDivLine3LabelDiv">
                            <p>声音是否吵闹</p>
                        </div>
                    </div>

                    <div  id="leftDivLine3EchartsDiv">
                        <div id="leftDivLine3EchartsContent"></div>
                    </div>
                    <div id="leftDivLine3Between">
                        <div v-if="noiseStatus !== null">
                            <p id="leftDivLine3BetweenP" :style="{ color: noiseStatus === '是' ? 'red' : 'green', fontSize: '25px'}">{{ noiseStatus }}</p>
                        </div>
                        <div id="leftDivLine3echats" ref="chart"></div>
                        <button class="ConfidenceLevelButton1" @click="toggleShowMoreLabel">{{ showAllLabel ? '隐藏' : '更多' }}</button>
                        <div id="leftDivLine3Noise" :style="{ maxHeight: showAllMessages ? 'auto' : '200px', overflowY: showAllMessages ? 'auto' : 'hidden',
                            display:showAllLabel ? 'block' : 'none'
                         }">
                            <div v-for="(msg, index) in visibleMessages" :key="index" 
                                :class="{ 'red-noise': isNoiseAboveThreshold(msg), 'blue-noise': !isNoiseAboveThreshold(msg) }">
                                {{ msg }}
                            </div>
                        </div>
                    </div>
                    
                    <!-- <p :config="plablevalue" style="color: aqua;font-size: 45px;padding-top: 15%;">否</p> -->
                </div>

                <div id="leftDivLine4">
                    <div id="leftDivLine4UpDiv">
                        <div id="leftDivLine4LabelDiv">
                            <p>多少人没戴头盔</p>
                        </div>
                    </div>
                    <div  id="leftDivLine4EchartsDiv">
                        <div id="leftDivLine4EchartsContent" v-if="specificLabelCount !== null">
                            <!-- 当 detections 数组的长度大于 0 且检测结果符合自定义条件时，显示一个红色字体、20像素字号的无序列表 -->
                            <div class="leftDivLine4Between">
                                <div :style="{ color: 'red', fontSize: '20px', maxHeight: showAllLabels ? 'auto' : '135px', overflowY: showAllLabels ? 'auto' : 'hidden' }">
                                    <div v-for="(label, index) in visibleLabels" :key="index">{{ label }}
                                        
                                    </div>
                                    
                                </div>
                                <button class="ConfidenceLevelButton2" @click="viewPicture">查看</button>
                                <button class="ConfidenceLevelButton" v-if="SumLabels.length > maxVisibleLabels" @click="toggleShowMoreLabels">{{ showAllLabels ? '隐藏' : '更多' }}</button>
                            </div>
                        </div>
                    </div>
                </div>

            </div>
                
            <div id="midDiv">
                <div v-if="warming">
                    
                </div>
                <div class="video" id="video_one">
                    <img class="ai_vision" :src="streamUrl" alt="Video Stream"/>
                </div>
            </div>

            <div id="rightDiv">
                <div id="rightDivRow1">
                    <div id="rightDivRow1Div">
                        <p>当前插入的广告</p>
                    </div>
                </div>
                <div id="rightDivRow2Left">
                    <img :src="currentImage" alt="Slideshow Image" />
                </div>
                   
                <!--植入按钮-->
                <div id="orderBtnDiv">
                    <div class="btn" @click="triggerFileInput">植入广告</div>
                    <input type="file" ref="fileInput" @change="handleFileChange" accept="image/*" style="display: none;" />
                </div>

                <div id="rightDivRow4">
                    <div id="rightDivRow4Div">
                        <p>环境监测</p>
                    </div>
                </div>
                <div id="rightDivRow5">
                    <!--温度-->
                    <div class="rightDivRow5Item">
                        <div class="rightDivRow5ItemLabel">
                            <p>温度</p>
                        </div>
                        <div class="rightDivRow5ItemProgress">
                            <dv-percent-pond class="rightDivRow5ItemProgressBar" :config="rightDivRow5Item1Config" style="width:80%;height:40%;" />
                        </div>
                        <div class="rightDivRow5ItemVal">
                            <p>{{ (rightDivRow5Item1Config.value).toFixed(2)}} ℃</p>
                        </div>
                    </div>

                    <div class="rightDivRow5Item">
                        <div class="rightDivRow5ItemLabel">
                            <p>湿度</p>
                        </div>
                        <div class="rightDivRow5ItemProgress">
                            <dv-percent-pond class="rightDivRow5ItemProgressBar" :config="rightDivRow5Item2Config" style="width:80%;height:40%;" />
                        </div>
                        <div class="rightDivRow5ItemVal">
                            <p>{{ (rightDivRow5Item2Config.value).toFixed(2)}} %</p>
                        </div>
                    </div>

                    <div class="rightDivRow5Item">
                        <div class="rightDivRow5ItemLabel">
                            <p>光照度</p>
                        </div>
                        <div class="rightDivRow5ItemProgress">
                            <dv-percent-pond class="rightDivRow5ItemProgressBar" :config="rightDivRow5Item3Config" style="width:80%;height:40%;" />
                        </div>
                        <div class="rightDivRow5ItemVal">
                            <p>{{ (rightDivRow5Item3Config.value).toFixed(0) }} Lx</p>
                        </div>
                    </div>

                    <div class="rightDivRow5Item">
                        <div class="rightDivRow5ItemLabel">
                            <p>气压</p>
                        </div>
                        <div class="rightDivRow5ItemProgress">
                            <dv-percent-pond class="rightDivRow5ItemProgressBar" :config="rightDivRow5Item4Config" style="width:80%;height:40%;" />
                        </div>
                        <div class="rightDivRow5ItemVal">
                            <p>{{ (rightDivRow5Item4Config.value).toFixed(2) }}KPa</p>
                        </div>
                    </div>

                

                    <div class="rightDivRow5Item">
                        <div class="rightDivRow5ItemLabel">
                            <p>噪音</p>
                        </div>
                        <div class="rightDivRow5ItemProgress">
                            <dv-percent-pond class="rightDivRow5ItemProgressBar" :config="rightDivRow5Item6Config" style="width:80%;height:40%;" />
                        </div>
                        <div class="rightDivRow5ItemVal">
                            <p>{{ (rightDivRow5Item6Config.value) }} db</p>
                        </div>
                    </div>

                </div>

            </div>


            <div id="bottomDiv">
                
                <div id="bottomDivLift">
                    <dv-decoration-11 class="bottomDivLift1Lable">
                        <div style="font-size: 19px;color: white;">按下报警</div>
                    </dv-decoration-11>
                    <dv-border-box1>
                        <div style="height: 100%;display: flex;flex-direction: row;align-items: center;">
                            <div style="width: 70%;height:100%;display: flex;flex-direction: column">
                            </div>
                        </div>
                    <div class="policeLable" @click="showConfirmDialog = true">报警</div>
                    <div v-if="showConfirmDialog" class="modal-overlay">
                        <div class="modal">
                            <p style="color: black;">你确定要报警吗？</p>
                            <div class="modal-buttons">
                                <button @click="confirmAlarm">是</button>
                                <button @click="cancelAlarm">否</button>
                            </div>
                        </div>
                    </div>
                </dv-border-box1>
            </div>

                <div id="bottomDivRight">
                    <dv-decoration-11 style="position: absolute;width:360px;height:60px;top: -25px;right: 80px;z-index: 10">
                        <div style="font-size: 19px;color: white;">灯的开关</div>
                    </dv-decoration-11>
                    
                    <dv-border-box1>
                        <div style="height: 100%;display: flex;flex-direction: row;align-items: center;">
                            <div style="width: 70%;height:100%;display: flex;flex-direction: column">
                                
                            </div>
                           
                            <div class="button-container">
                               <!-- putPossition是一个方法 -->
                               <div class="btn1" @click="handleDimss" :style="{color: SwitchAutomaticManual.value === '手动' ? 'red' : '#4cc9f0', 
                               fontSize: '45px', cursor:SwitchAutomaticManual.value==='手动' ? 'pointer' : 'default',
                               border:`1px solid ${SwitchAutomaticManual.value === '自动' ? '#4cc9f0' : 'red'}`}" 
                               :disabled="SwitchAutomaticManual.value === '自动'">
                                {{SwitchAutomaticManual.value}}
                            </div>
                               <div class="btn" @click="handleOpen">开</div>
                               <div class="btn" @click="handleClose">关</div>
                               <div class="btn" @click="handleDim">暗</div>
                            </div>
                        </div>
                    </dv-border-box1>
                </div>
            </div>

        </div>
    </dv-full-screen-container>

</template>

<script setup>
    import { onMounted, onBeforeUnmount, ref, reactive, onUnmounted,computed,watchEffect ,nextTick} from 'vue'
    import '../assets/js/dayjs/dayjs.min';
    import '../assets/js/dayjs/zh-cn';
    import * as echarts from 'echarts'
	import * as mqtt from 'mqtt/dist/mqtt.min.js';
    import axios from 'axios';
    import { useStore } from 'vuex';
    import { ElButton, ElMessageBox } from 'element-plus';

    

    const scale_x = ref(1);
    const scale_y = ref(1);


    const domDayDiv =ref("");
    const domTimeDiv =ref("");

    const map_earth = ref(null);
    const weather = ref(null);
    let noiseValue11=0;

    const store = useStore();
    const currentIndex = ref(0);
    const fileInput = ref(null);
    const noiseStatus = ref('否');
    const noiseLevelColor = ref(0)
    const noiseLevel = ref(0);
    const messages = ref([]);
    
    const showAllMessages = ref(false);
    const maxVisibleMessages = 5;
    const warming =ref(false);

    const images = store.state.images;


    const visibleMessages = computed(() => {
        return showAllMessages.value ? messages.value : messages.value.slice(-maxVisibleMessages);
    });

    const isNoiseAboveThreshold = (msg) => {
        const noiseValue = parseInt(msg.split(' ')[1]);
        return noiseValue > 40;
    };

    const currentImage = computed(() => images[currentIndex.value]);
    //当value的值和images一样时，就更新为0重新开始
    const updateImage = () => {
        currentIndex.value = (currentIndex.value + 1) % images.length;
    };
    //创建文件资源管理器
    const triggerFileInput = () => {
        fileInput.value.click();
    };


    const viewPicture = () => {
        window.open('/picture', '_blank');
    };

   
    const warming1 =()=>{
        warming.value=false;
    }

    const handleFileChange = (event) => {
        // 获取选择的第一个文件
        const file = event.target.files[0];
        if (file) {
            // 异步读取 File 或 Blob 对象
            const reader = new FileReader();
            reader.onload = (e) => {
                const image = e.target.result;
                store.commit('addImage', image);
                store.commit('setAdImage', image); // 更新 store 中的 adImage 
                console.log(images)
                console.log('images:', store.state.images);
                console.log('adImage:', store.state.adImage);
            };
            reader.readAsDataURL(file);
        }
    };
    console.log(images)

    let intervalId;

    onMounted(() => {
        intervalId = setInterval(updateImage, 3000); // 每3秒更新一次
    });

    onUnmounted(() => {
        clearInterval(intervalId); // 组件卸载时清除计时器
    });

    //echarts

    // 初始化引用和图表数据
    const chart = ref(null);
    let myChart = null;
    const data = {
        xAxisData: [],
        seriesData: []
    };

    // 更新图表函数
    const updateChart = (value) => {
        const now = new Date();
        data.xAxisData.push(now.toLocaleTimeString().replace(/^\D*/, ''));
        data.seriesData.push(value);

        if (data.xAxisData.length > 20) {
            data.xAxisData.shift();
            data.seriesData.shift();
        }     

        myChart.setOption({
            xAxis: {
                type: 'category',
                data: data.xAxisData
            },
            yAxis: {
                type: 'value'
            },
            series: [{
                data: data.seriesData,
                type: 'line',
                smooth: true,
                areaStyle: {}
            }]
        });
    };

    // 初始化 ECharts 图表
    const initChart = () => {
        if (chart.value) {
            myChart = echarts.init(chart.value);

            myChart.setOption({
                tooltip: {
                    trigger: 'axis'
                },
                xAxis: {
                    type: 'category',
                    data: data.xAxisData
                },
                yAxis: {
                    type: 'value'
                },
                series: [{
                    data: data.seriesData,
                    type: 'line',
                    smooth: true,
                    areaStyle: {}
                }]
            });
        }
    };
    
    watchEffect(() => {
        if (warming.value) {
            ElMessageBox.alert('灯杆报警按钮被按下，可能发生紧急情况', '警告', {
                confirmButtonText: '确定',
                type: 'warning',
                onClose: () => {
                    warming.value = false; // 弹窗关闭后可以重置 warming 状态
                    console.log("弹窗已关闭，warming 状态已重置。")
                }
            });
        }
    });

    //mqtt连接
    
    const showConfirmDialog = ref(false);
    const url = 'wss://iot-06z00g2e8e8102s.mqtt.iothub.aliyuncs.com:443/'
    //const url = 'iot-06z00g2e8e8102s.mqtt.iothub.aliyuncs.com'
	// 创建客户端实例
	const options = {
	// Clean session
		clean: true,
		connectTimeout: 4000,
		// 认证信息
        clientId: 'k1cxnMiabUP.wangye|securemode=2,signmethod=hmacsha256,timestamp=1717060789639|',
        //clientId: 'k1cxnMiabUP.test1|securemode=2,signmethod=hmacsha256,timestamp=1716981401623|',
		username: 'wangye&k1cxnMiabUP',
        //username: 'test1&k1cxnMiabUP',
		password: '0fc793d2991c24adb9503f50cce7624079f8fc21ff193d5c0c4999fa77fbf305',
        //password: '299c05df5c23a7d69fd80cf94e132d4759d501a95d6b9cc8941589ea4dbf54da',
	}
    
    const SwitchAutomaticManual = reactive({
        value: '自动'
    });
    function confirmAlarm() {
    // 确认报警的处理逻辑
    putPossition1();
    showConfirmDialog.value = false;
    }

    function cancelAlarm() {
    // 取消报警的处理逻辑
    showConfirmDialog.value = false;
    }
    function handleOpen() {
        console.log('开 button clicked');
        client.publish('/k1cxnMiabUP/wangye/user/ght1', '2');
        setToAutomatic();
    }
    function handleClose() {
        console.log('关 button clicked');
        client.publish('/k1cxnMiabUP/wangye/user/ght1', '0');
        setToAutomatic();
    }
    function handleDim() {
        console.log('暗 button clicked');
        client.publish('/k1cxnMiabUP/wangye/user/ght1', '1');
        setToAutomatic();
    }
    function setToAutomatic(){
        SwitchAutomaticManual.value = '手动';
    }
    function setToAutomatic1(){
        SwitchAutomaticManual.value = '自动';
    }
    const handleDimss= () =>{
        console.log('手动变自动')
        client.publish('/k1cxnMiabUP/wangye/user/ght1', '4');
        setToAutomatic1();
        // if (SwitchAutomaticManual.value === '手动') {
        //     SwitchAutomaticManual.value = '自动';
        // }
    };
    
    // 定义props
    const apiKey = '8f9ceff99bdcf0734b03fd1d1f6b7ef4';
    //判断是否为数组，并且统计特定标签的数量，同时调用ckplayer的库，已经进行前后端的交互
    const streamUrl = ref('http://172.20.10.13:8080/stream');
    // const streamUrl = ref('https://frp-now.top:21507/stream');
    const detections = ref([]);
    const specificLabelCount = ref(0);
    const screenshotUrl = ref(null);
    const SumLabel = ref(0);

    const SumLabels = ref([]); // 用于存储所有检测到的标签
    const showAllLabels = ref(false);
    const showAllLabel = ref(false);
    const maxVisibleLabels = 6;

    
    const fetchDetections = async () => {
        try {
            const response = await axios.get('http://172.20.10.13:8080/detections');
            const data = response.data;
            console.log('Fetched data:', data);
            if (Array.isArray(data.detections)) {
                detections.value = data.detections;

                // 统计特定标签的数量
                
                const label = ' No Headpiece';
                const count = data.detections.reduce((acc, detection) => {
                    if (detection.class === label) {
                            acc++;
                    }
                    return acc;
                }, 0);
                
                specificLabelCount.value = count;
                SumLabel.value += count;
                SumLabels.value.push(`No Headpiece: ${SumLabel.value}`);
                console.log('Specific label count:', specificLabelCount.value);

                // 更新截图 URL
                screenshotUrl.value = data.screenshot ? 'http://172.20.10.13:8080/screenshot' : null;
                console.log('Screenshot URL:', screenshotUrl.value);

                // 如果检测到特定标签，自动下载截图
                if (specificLabelCount.value > 0 && screenshotUrl.value) {
                    downloadScreenshot();
                }
            } else {
                console.error('Detections data is not an array:', data.detections);
            }
        } catch (error) {
            console.error('Error fetching detections:', error);
        }
    };

    const visibleLabels = computed(() => {
        return showAllLabels.value ? SumLabels.value : SumLabels.value.slice(-maxVisibleLabels);
    });

    const toggleShowMoreLabels = () => {
        showAllLabels.value = !showAllLabels.value;
    };
    const toggleShowMoreLabel =()=>{
        showAllLabel.value= !showAllLabel.value
    }

    // 记得调用 fetchDetections 方法以便获取数据
    fetchDetections();

    
    // 下载图片
    const downloadScreenshot = async () => {
    try {
        const response = await axios.get(screenshotUrl.value, { responseType: 'blob' });
        if (response.status === 200 && response.data) {
            const url = window.URL.createObjectURL(new Blob([response.data]));
            const link = document.createElement('a');
            link.href = url;
            link.setAttribute('download', 'screenshot.jpg');
            document.body.appendChild(link);
            link.click();
            document.body.removeChild(link);

            // 告诉后端图片已经下载完毕
            await axios.post('http://172.20.10.13:8080/open-folder');
        } else {
            console.error('Failed to download screenshot, status code:', response.status);
        }
    } catch (error) {
        console.error('Error downloading screenshot:', error);
    }
};
    

    onMounted(() => {
        fetchDetections();
        setInterval(fetchDetections, 5000);
    });

    setInterval(()=>{
        let donghua = ["animate__animated animate__shakeX","animate__animated animate__flash",
            "animate__animated animate__pulse","animate__animated animate__rubberBand",
            "animate__animated animate__headShake","animate__animated animate__swing",
            "animate__animated animate__jello","animate__animated animate__heartBeat"];
        let num = Math.floor( Math.random() * donghua.length );
        document.getElementById("headPartH1").className = donghua[num]
    },5000);



    
    const city='Wuhan'
    onMounted(async () => {
            try {
                const response = await axios.get(`https://api.openweathermap.org/data/2.5/weather?q=${city}&appid=${apiKey}&units=metric&lang=zh_cn`);
                weather.value = response.data;
            } catch (error) {
                console.error('获取天气数据失败:', error);
            }
        });

    
    const bjlablevalue = reactive({
        value: '否'
    });
    
    
    const rightDivRow5Item1Config = reactive({
        value: 0,                  //温度值-20～60度→百分比
        borderWidth: 2,
        borderRadius: 5,
        borderGap: 2,
    });
    const rightDivRow5Item2Config = reactive({
        value: 0,                  //湿度值0～100→百分比
        borderWidth: 2,
        borderRadius: 5,
        borderGap: 2,
    });
    const rightDivRow5Item3Config = reactive({
        value: 0,                  //光照度值0～10000→百分比
        borderWidth: 2,
        borderRadius: 5,
        borderGap: 2,
    });
    const rightDivRow5Item4Config = reactive({
        value: 0,                  //CO浓度值0～100→百分比
        borderWidth: 2,
        borderRadius: 5,
        borderGap: 2,
    });
    const rightDivRow5Item5Config = reactive({
        value: 0,                  //PM2.5值0～1000→百分比
        borderWidth: 2,
        borderRadius: 5,
        borderGap: 2,
    });
    const rightDivRow5Item6Config = reactive({
        value: 0,                  //噪音值0～100→百分比
        borderWidth: 2,
        borderRadius: 5,
        borderGap: 2,
    });
    const rightDivRow5Item7Config = reactive({
        value: 0,                  //电流值0～10A→百分比
        borderWidth: 2,
        borderRadius: 5,
        borderGap: 2,
    });

    //报警函数
    let timeoutId1;
    function putPossition1() {
        client.publish('/k1cxnMiabUP/wangye/user/ght1', '3');
        console.log('3')
        bjlablevalue.value = '是';

        function setPossition1() {
            bjlablevalue.value = '否';
        }

        timeoutId1 = setTimeout(() => {
            setPossition1();
            clearTimeout(timeoutId1);
        }, 5000); 
    }



    const loadTime = () =>{
        domTimeDiv.value = dayjs().locale("zh-cn").format('HH:mm:ss');
        domDayDiv.value = dayjs().locale("zh-cn").format('MM月DD日 dddd');
    };
    loadTime();

    const t1 = setInterval(loadTime, 500);
    let t2 = null;
    let t3 = null;
    let t4 = null;       // 中间地图1 实心地图及闪烁点
    let t5 = null;       // 中间地图2
    let t4_map1 = null;  // t4中的循环闪烁问题
    let t5_map1 = null;  // t5中的循环闪烁问题
    let t6 = null;       // 中间地图1

	let client = null;	//mqtt客户端
	
	
    
    onMounted(()=> {
        t4 = setTimeout(() => {
                

            let noiseValue11 = 1;
			client = mqtt.connect(url, options)
			client.on('connect', function () {
			    console.log('Connected')
			   
			})
			// 接收消息
			client.on('message', function (topic, message) {
			  // message is Buffer
			  console.log(message.toString())
			  message = JSON.parse(message)
              if (typeof message === 'object' && message !== null){
                message.Noise=(message.Noise/4096*90).toFixed(2)
                console.log(`message.Noise: ${message.Noise}, type: ${typeof message.Noise}`);
                updateChart(message.Noise); // 更新图表
              }
               

                if (message.ring === 1) {
                    warming.value = true; 
                }

                if (message.Noise !== undefined) {
                    noiseLevel.value = message.Noise;
                    noiseStatus.value = message.Noise > 40 ? '是' : '否';
                    messages.value.push(`噪音水平: ${message.Noise} dB`);
                }
                // 自动滚动效果
                if (messages.value.length > maxVisibleMessages) {
                    const div = document.getElementById('leftDivLine3EchartsDiv');
                    div.scrollTop = div.scrollHeight;
                }
                
                if(message.ring===1){
                    bjlablevalue.value = '是';
                }else{
                    bjlablevalue.value = '否';
                }
			      // 温度变化
			      if(message.Temperation != undefined){
			    	rightDivRow5Item1Config.value = (message.Temperation)/100;
                    console.log( rightDivRow5Item1Config.value );
			      }
			      // 湿度变化
			      if(message.Humidity != undefined){
			    	rightDivRow5Item2Config.value = (message.Humidity)/100;

			      }
              
			      // 光照度变化
			      if(message.Light != undefined){
			      	rightDivRow5Item3Config.value = message.Light;

			      }
			      //气压
			      if(message.Atmospheric_Pressure != undefined){
			      	rightDivRow5Item4Config.value = message.Atmospheric_Pressure/100000;
			      }
			      // PM2.5
			        //   if(message.Enviroment_Pm25 != undefined){
			        //   	rightDivRow5Item5Config.value = message.Enviroment_Pm25 / 10;
			      // 噪音
			      if(message.Noise != undefined){
			      	rightDivRow5Item6Config.value = message.Noise;
			      }
              
			     
			  
			})
			
			initChart();

			
        }, 500);
        

        // 
        //di qiu
    });
    onBeforeUnmount (() => {
        clearInterval(t1);
        clearInterval(t2);
        clearInterval(t3);
        clearInterval(t4);
        clearInterval(t5);
        if (t4_map1 != null) clearInterval(t4_map1);
        if (t5_map1 != null) clearInterval(t5_map1);


        if (browserRedirect().toString() == "Linux") {
            scale_x.value = window.innerWidth / 1920 ;
            scale_y.value = window.innerHeight / 1020 ;

            scale_x.value = 0.2;
            scale_y.value = 0.2 ;
            //alert(scale_x.value)
        }
		if(client != null){
			client.end()
		}
    });
    

//检测操作系统
    function browserRedirect(){
        var sUserAgent = navigator.userAgent;
        var isWin = (navigator.platform == "Win32") || (navigator.platform == "Windows");
        var isMac = (navigator.platform == "Mac68K") || (navigator.platform == "MacPPC") || (navigator.platform == "Macintosh") || (navigator.platform == "MacIntel");
        if (isMac) return "Mac";
        var isUnix = (navigator.platform == "X11") && !isWin && !isMac;
        if (isUnix) return "Unix";
        var isLinux = (String(navigator.platform).indexOf("Linux") > -1);
        if (isLinux) return "Linux";
        if (isWin) {
            var isWin2K = sUserAgent.indexOf("Windows NT 5.0") > -1 || sUserAgent.indexOf("Windows 2000") > -1;
            if (isWin2K) return "Win2000";
            var isWinXP = sUserAgent.indexOf("Windows NT 5.1") > -1 || sUserAgent.indexOf("Windows XP") > -1;
            if (isWinXP) return "WinXP";
            var isWin2003 = sUserAgent.indexOf("Windows NT 5.2") > -1 || sUserAgent.indexOf("Windows 2003") > -1;
            if (isWin2003) return "Win2003";
            var isWinVista= sUserAgent.indexOf("Windows NT 6.0") > -1 || sUserAgent.indexOf("Windows Vista") > -1;
            if (isWinVista) return "WinVista";
            var isWin7 = sUserAgent.indexOf("Windows NT 6.1") > -1 || sUserAgent.indexOf("Windows 7") > -1;
            if (isWin7) return "Win7";
            var isWin10 = sUserAgent.indexOf("Windows NT 10") > -1 || sUserAgent.indexOf("Windows 10") > -1;
            if (isWin10) return "Win10";
        }
        return "other";
    }
    
</script>

<style scoped>
#bg_div {
    position: absolute;
    width: 100%;
    height: 100%;
    background-image: url("../assets/images/bg.png");
    background-size: cover;
    overflow-x: hidden;
    transform-origin: 0px 0px;
    filter: hue-rotate(0deg) saturate(100%) brightness(100%) contrast(100%) opacity(100%) grayscale(0%);
    margin: 0;
    border: 0;
    padding: 0;
    font-family: PingFang-SC, "Helvetica Neue", Helvetica, "Hiragino Sans GB", "Microsoft YaHei", 微软雅黑, Arial, sans-serif !important;
    transform: scale(scale_x, scale_y);
}

#CNweather {
    position: absolute;
    width: 100%;
    top: 00px;
    font-size: 16px;
    color: rgba(255, 255, 255,1);
    line-height: 60px;
    text-align: center;
    font-weight: normal;
    letter-spacing: 2px;
}

.weather-info {
    display: flex;
    justify-content: space-around;
    margin: 0 0px;
}
.label {
    margin-right: 1px; /* 调整城市名称和后面值之间的距离 */
}

.Leftinfo-item{
    font-size: 15px;
    top: 40px;
    left: -150px;
    position: relative;
}

.Rightinfo-item {
    font-size: 15px;
    top: 40px;
    right: 500px;
    position: relative;
}


#headPart {
    overflow: hidden;
    position: absolute;
    left: 4px;
    top: 0;
    width: 1916px;
    height: 101px;
    z-index: 2;
    background-image: url("../assets/images/headPartBg.png");
    animation: animate_head 0.5s ease-in;
}
@keyframes animate_head {
    0%
    {
        transform: translateY(-101px);
    }
    100%
    {
        transform: translateY(0px);
    }
}
#headPartWord {
    color: #ffffff;
    display: flex;
    text-align: center;
    line-height: 1;
    font-style: normal;
    justify-content: center;
    align-items: center;
    font-family: "微软雅黑";
    font-size: 10px;
    letter-spacing: 4px;
}


#timeAndDay {
    display: flex;
    flex-direction: row;
    padding-left: 30px;
    overflow: hidden;
    position: absolute;
    left: 12.52px;
    top: 22.84px;
    width: 488.83px;
    height: 360px;
    z-index: 6;
    transform: rotate(0deg);
    animation: animate_dayTime 1.5s ease-in;
}
@keyframes animate_dayTime {
    0%
    {
        opacity: 0.0;
    }
    100%
    {
        opacity: 1.0;
    }
}


#timeDiv {
    font-size: 26px;
    color: rgba(255, 255, 255,1);
    line-height: 60px;
    text-align: center;
    font-weight: normal;
    letter-spacing: 2px;
}

#dayDiv {
    margin-left: 30px;
    font-size: 14px;
    color: rgba(255, 255, 255, 0.77);
    line-height: 60px;
    text-align: center;
    font-weight: normal;
    letter-spacing: 2px;
}

#leftDiv {
    /* overflow: hidden; */
    position: absolute;
    left: 24px;
    top: 90px;
    width: 397px;
    height: 807px;
    z-index: 4;
    background: url("../assets/images/leftBg1.png");
    animation: animate_leftDiv  0.5s ease-in;
}
@keyframes animate_leftDiv {
    0%
    {
        transform: translateX(-397px);
    }
    100%
    {
        transform: translateX(0px);
    }
}

#leftDivLine1 {
    overflow: hidden;
    position: absolute;
    left: 6px;
    top: 17.27px;
    width: 380px;
    height: 39px;
    z-index: 2;
    background: url("../assets/images/leftDivLine1Bg.png");
}

#leftDivLine1LabelDiv, #rightDivRow1Div, #rightDivRow4Div {
    line-height: 1.5;
    font-size: 16px;
    text-align: center;
    font-weight: bold;
    font-style: normal;
    display: flex;
    flex-wrap: wrap;
    justify-content: flex-start;
    color: #ffffff;
    margin-left: 50px;
    margin-top: -12px;
}

#leftDivLine2 {
    display: flex;
    flex-direction: row; /*从左到右的方向排序*/
    justify-content: space-around;
    overflow: hidden;
    position: absolute;
    top: 81.37px;
    width: 100%;
    height: 110px;
    z-index: 5;
}



#leftDivLine3 {
    /* overflow: hidden; */
    position: absolute;
    left: 6px;
    top: 231px;
    width: 380px;
    height: 228.76px;
    z-index: 6;
    display: flex;
    flex-direction: column;
    justify-content: flex-start;
}

#leftDivLine3UpDiv{
    overflow: hidden;
    position: absolute;
    left: 0;
    top: 0;
    width: 380px;
    height: 39px;
    z-index: 4;
    background: url("../assets/images/leftDivLine1Bg.png");
}

#leftDivLine3LabelDiv {
    line-height: 1.5;
    font-size: 16px;
    text-align: center;
    font-weight: bold;
    font-style: normal;
    display: flex;
    flex-wrap: wrap;
    justify-content: flex-start;
    align-items: center;
    color: #ffffff;
    width: 380px;
    height: 39px;
    margin-left: 50px;
    margin-top: -12px;
}
#leftDivLine3Noise{
    position: absolute;
    width: 100px;
    left: 300px;
}
.red-noise {
  color: red;
}
.blue-noise {
  color: #4cc9f0;
}
#leftDivLine3echats{
    position: absolute;
    top: 20px;
    width: 100%; 
    height: 200px;
}
.ConfidenceLevelButton1{
    position: absolute;
    top: 40px;
    left: 300px;
    width: 50px;
    height: 30px;
    z-index: 7;
}
#leftDivLine3EchartsDiv{
    overflow: hidden;
    position: absolute;
    left: 1.53px;
    top: 33.38px;
    width: 360px;
    height: 195.38px;
    z-index: 8;
}

#leftDivLine3EchartsContent{
    position: relative;
    width: 360px;
    height: 195px;
    padding: 0;
    margin: 0;
    border-width: 0;
    cursor: default;
}

.leftDivLine3More{
    position: absolute;
    top:65px;
    right: 50px;
}
#leftDivLine3Between{
    position: absolute;
    top: 30px;
    left: 40px;
    width: 300px;
    height: 290px;
}

#leftDivLine4 {
    overflow: hidden;
    position: absolute;
    left: 5.68px;
    top: 509px;
    width: 380.32px;
    height: 222.07px;
    z-index: 7;
    display: flex;
    flex-direction: column;
    justify-content: flex-start;
}
.leftDivLine4Between{
    position: absolute;
    top: 40px;
    left: 60px;
    width: 300px;
    height: 90px;
}

.ConfidenceLevelButton{
    position: absolute;
    font-size: 18px;
    top: -25px;
    right: -5px;
    border-radius: 10px;
}
.ConfidenceLevelButton2{
    position: absolute;
    font-size: 18px;
    top: -25px;
    left: -5px;
    border-radius: 10px;
}
#leftDivLine4UpDiv{
    overflow: hidden;
    position: absolute;
    left: 0;
    top: 0;
    width: 380px;
    height: 39px;
    z-index: 4;
    background: url("../assets/images/leftDivLine1Bg.png");
}
#leftDivLine4LabelDiv {
    line-height: 1.5;
    font-size: 16px;
    text-align: center;
    font-weight: bold;
    font-style: normal;
    display: flex;
    flex-wrap: wrap;
    justify-content: flex-start;
    align-items: center;
    color: #ffffff;
    width: 380px;
    height: 39px;
    margin-left: 50px;
    margin-top: -12px;
}
#leftDivLine4EchartsDiv{
    overflow: hidden;
    position: absolute;
    left: 0;
    top: 31.3px;
    width: 366.16px;
    height: 190.77px;
    z-index: 8;
}

#leftDivLine4EchartsContent{
    
    position: relative;
    width: 366px;
    height: 191px;
    padding: 0;
    margin: 0;
    border-width: 0;
    cursor: default;
    z-index: 9;
}

#midDiv{
    overflow: hidden;
    position: absolute;
    left: 399.75px;
    top: 10px;
    width: 1121px;
    height: 1049px;
    z-index: 1;
    /*background-color: #007aff;*/
    background: url("../assets/images/midBg.png");
    animation: animate_midDiv  1.5s ease-in;
}

@keyframes animate_midDiv {
    0%, 60%, 75%, 90%, 100%
    {
        transition-timing-function: cubic-bezier(0.215, 0.61, 0.355, 1);/*贝塞尔曲线*/
    }
    0%
    {
        opacity: 0;/*元素完全透明*/
         transform: translate3d(0, -3000px, 0); /*元素在 y 轴上移出 3000 像素 */
    }
    60%
    {
        opacity: 1;
        transform: translate3d(0, 25px, 0);
    }
    75%
    {
        transform: translate3d(0, -10px, 0);
    }
    90%
    {
        transform: translate3d(0, 5px, 0);
    }
    100%
    {
        transform: none;
    }
}

#video_one{
    overflow: hidden;
    position: absolute;
    /*left: 23.31px;*/
    /*top: -70px;*/
    width: 950px;
    height: 700px;
    left: 100px;
    top: 100px;
    /*width: 1059.99px;*/
    /*height: 1058.46px;*/
    z-index: 4;
}
.ai_vision{
    width: 90%; 
    max-width: 800px;
    border-radius: 50%;
}
#midMapDiv2{
    overflow: hidden;
    position: absolute;
    left: 31.31px;
    top: -67px;
    width: 1059.99px;
    height: 1058.46px;
    z-index: 3;
}

#rightDiv{
    overflow: hidden;
    position: absolute;
    left: 1501px;
    top: 90px;
    width: 397px;
    height: 807px;
    z-index: 5;
    background: url("../assets/images/rightBg.png");
}
#rightDivRow1, #rightDivRow4{
    overflow: hidden;
    position: absolute;
    left: 3.81px;
    top: 22.27px;
    width: 383px;
    height: 35px;
    z-index: 2;
    background: url("../assets/images/rightDivRow1Bg.png");
}
#rightDivRow4{
    top: 350px;
    height: 35px;
    z-index: 3;
    background: url("../assets/images/rightDivIcon2.png");
}
#rightDivRow2Left, #rightDivRow2Right{
    overflow: hidden;
    position: absolute;
    left: 9px;
    top: 62px;
    width: 100%;
    height: 212px;
    z-index: 3;
}
#rightDivRow2Left img {
  max-width: 100%;
  max-height: 100%;
  object-fit: cover;
  transition: opacity 1s ease-in-out;
}

#orderBtnDiv{
    /*overflow: hidden;*/
    position: absolute;
    left: 0;
    top: 280px;
    width: 100%;
    height: 60px;
    z-index: 9;
    display: flex;
    justify-content: center;
    align-items: center;
    /*background-color: #007aff;*/
}
#rightDivRow5{
    overflow: hidden;
    position: absolute;
    left: 17.48px;
    top: 391.64px;
    width: 371px;
    height: 389.32px;
    z-index: 2;
    padding-top: 20px;
}
.rightDivRow5Item{
    overflow: hidden;
    left: 0;
    top: 0;
    width: 371px;
    height: 39px;
    z-index: 1;
    margin-bottom: 10px;
    display: flex;
    flex-direction: row;
    background: url("../assets/images/rightDivRow5ItemBg.png");
}

.rightDivRow5ItemLabel{
    height: 100%;
    width: 20%;
    display: flex;
    justify-content: flex-end;
    align-items: center;
    line-height: 1.5;
    font-size: 16px;
    text-align: center;
    font-weight: normal;
    color: rgba(255, 152, 0, 1);
    z-index: 1;
}
.rightDivRow5ItemProgress{
    height: 100%;
    width: 55%;
    display: flex;
    flex-direction: row;
    justify-content: center;
    align-items: center;
    /*background-color: green;*/
    z-index: 1;
}
.rightDivRow5ItemVal{
    height: 100%;
    width: 25%;
    display: flex;
    flex-direction: row;
    justify-content: flex-start;
    align-items: center;
    line-height: 1.5;
    font-size: 16px;
    text-align: center;
    font-weight: normal;
    color: rgba(16, 150, 193, 1);
    /*background-color: yellowgreen;*/
    z-index: 1;
}

#bottomDiv{
    overflow: hidden;
    position: absolute;
    left: 0;
    top: 811px;
    width: 1920px;
    height: 269px;
    z-index: 3;
    background: url("../assets/images/bottomBg.png");
}
#bottomDivLift{
    position: absolute;
    left: 380.53px;
    top: 49px;
    width: 520px;
    height: 200px;
    z-index: 7;
	display: flex;
	flex-direction: row;
    justify-content: flex-end;

}
.bottomDivLift1Lable{
    position: absolute;
    width:360px;
    height:60px;
    top: -25px;
    right: 80px;
    z-index: 10;
}
.policeLable{
    position: absolute;
    color: black;
    top: 80px;
    left: 150px;
    width: 35%;  
    font-size: 20px;
    display: flex;
    justify-content: center;
    align-items: center;  
    font-weight: bold;
    cursor: pointer; 
    padding: 10px;  
    background-color: #fd0000;  
    border: 1px solid #f50505;  
    border-radius: 5px;  
    margin: 0 10px;  
}
.policeLable:hover {
    background-color: #e0e0e0;
    border: 1px solid #e0e0e0; 
}
.modal-overlay {
    position: relative;
    position: fixed;
    top: 0;
    left: 0;
    width: 100%;
    height: 100%;
    background: rgba(0, 0, 0, 0.5);
    display: flex;
    justify-content: center;
    align-items: center;
}
.modal {
    position: absolute;
    top: 400px;
    right:1200px ;
    width: 25%;
    height: 15%;
    background: rgba(102,175,233,.6);
    padding: 20px;
    border-radius: 5px;
    text-align: center;
}
.modal-buttons {
    margin-top: 20px;
    
}
.modal-buttons button {
    margin: 0 10px;
}
#bottomDivRight{
    /*overflow: hidden;*/
    position: absolute;
    left: 945px;
    top: 49px;
    width: 520px;
    height: 200px;
    z-index: 2;
}
.button-container {
    position: relative;
    right: 38px;
    width: 2000px; 
    height: 100vh;  
    display: flex;
    flex-direction: row;  
    justify-content: center;  /* 水平方向居中 */
    align-items: center;  /* 垂直方向居中 */
}
.btn {
    width: 35%;  
    font-size: 20px;
    display: flex;
    justify-content: center;
    align-items: center;  
    font-weight: bold;
    cursor: pointer;  /* 添加手型光标 */
    padding: 10px;  /* 添加内边距以增加点击区域 */
    background-color: #f0f0f0;  /* 按钮背景颜色 */
    /*border: 1px solid #ccc;   按钮边框 */
    border-radius: 5px;  /* 按钮圆角 */
    margin: 0 10px;  /* 设置左右外边距 */
}
.btn1{
    width: 35%;
    font-size: 20px;
    border: 1px solid #4cc9f0;
}
#bottomDivLiftXDiv, #bottomDivLiftYDiv, #bottomDivLiftZDiv{
    width: 30%;
    display: flex;
    flex-direction: column;
    justify-content: flex-end;
    position: relative
}
#bottomDivLiftXDiv{
    right: -20px;
}
#bottomDivLiftZDiv{
    right: 20px;
}

.bottomDivLiftXDivLabel{
    width: 100%;
    height: 15%;
    position: relative;
    bottom: -50px
}
:deep(.rightDivRow5ItemProgressBar text){
    display: none;
}
:deep(.dv-percent-pond text){
    font-size: 18px;
}
.btn {
    position: relative;
    border: 1px solid;
    background-color: transparent;
    /*text-transform: uppercase;*/
    font-size: 16px;
    padding: 5px 20px;
    font-weight: 300;
    color: #4cc9f0;
    top: 5px;
    cursor: pointer;  /*鼠标悬停变小手*/
    -moz-user-select: none;
    -webkit-user-select: none;
    -ms-user-select: none;
    -khtml-user-select: none;
    user-select: none;
}
.btn:hover {
    color: white;
    border: 0;
    background-color: #4cc9f0;
    -webkit-box-shadow: 10px 10px 99px 6px rgba(76,201,240,1);
    -moz-box-shadow: 10px 10px 99px 6px rgba(76,201,240,1);
    box-shadow: 10px 10px 99px 6px rgba(76,201,240,1);
}




</style>



