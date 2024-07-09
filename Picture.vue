<template>
  <div>
    <h1>骑行者未佩戴头盔后台页面</h1>
    <div class="filter-container">
      <label for="startDate">开始日期:</label>
      <input type="date" id="startDate" v-model="startDate" @change="filterImages" />
      <label for="endDate">结束日期:</label>
      <input type="date" id="endDate" v-model="endDate" @change="filterImages" />
    </div>
    <div v-if="filteredImageInfoList.length" class="image-container">
      <div v-for="imageInfo in filteredImageInfoList" :key="imageInfo.url" class="image-item">
        <img :src="imageInfo.url" alt="Downloaded Image" @error="removeImage(imageInfo.url)" @dblclick="openImageInNewTab(imageInfo.url)"/>
        <p>{{ imageInfo.downloadTime }}</p>
      </div>
    </div>
    <p v-else>等待图片下载...</p>
  </div>
</template>

<script setup>
import { ref, onMounted } from 'vue';
import axios from 'axios';

const imageInfoList = ref([]);
const filteredImageInfoList = ref([]);
const startDate = ref(null);
const endDate = ref(null);
const openImageInNewTab = (url) => {
  const viewerUrl = `/imageViewer.html?url=${encodeURIComponent(url)}`;
  window.open(viewerUrl, '_blank');
};


const checkForDownloadedImages = async () => {
  try {
    const response = await axios.get('http://localhost:8080/check-downloaded-images');
    if (response.data.imagesAvailable) {
      imageInfoList.value = response.data.imageInfoList;
      filterImages();
    }
  } catch (error) {
    console.error('Error checking for downloaded images:', error);
  }
};

const filterImages = () => {
  const start = new Date(startDate.value);
  const end = new Date(endDate.value);
  if (!startDate.value || !endDate.value) {
    filteredImageInfoList.value = imageInfoList.value;
    return;
  }

  filteredImageInfoList.value = imageInfoList.value.filter(imageInfo => {
    const downloadTime = new Date(imageInfo.downloadTime);
    return downloadTime >= start && downloadTime <= end;
  });
};

const removeImage = (url) => {
  imageInfoList.value = imageInfoList.value.filter(imageInfo => imageInfo.url !== url);
  filterImages();
};

onMounted(() => {
  setInterval(checkForDownloadedImages, 5000);
});
</script>

<style>
.filter-container {
  margin-bottom: 20px;
}

.image-container {
  display: flex;
  flex-wrap: wrap;
  gap: 10px; /* 间距可以根据需要调整 */
}

.image-item {
  flex: 1 1 auto; /* 允许图片占据可用空间，并在需要时换行 */
  max-width: 200px; /* 图片的最大宽度，可以根据需要调整 */
  text-align: center; /* 居中显示图片和时间 */
}

.image-item img {
  width: 100%; /* 确保图片宽度自适应容器 */
  height: auto; /* 确保图片保持比例 */
}
</style>
