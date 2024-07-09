import { createStore } from 'vuex';

const store = createStore({
  state: {
    images: [
      'src/assets/images/map/OIP.jpg',
      'src/assets/images/map/OIP(1).jpg',
      'src/assets/images/map/OIP(2).jpg'
    ],
    adImage: null
  },
  mutations: {
    addImage(state, image) {
      state.images.push(image);
    },
    setAdImage(state, image) {
      state.adImage = image;
    }
  },
  getters: {
    images: state => state.images,
    adImage: state => state.adImage,
    displayedImage: state => state.adImage ? state.adImage : state.images[0]
  }
});

export default store;
