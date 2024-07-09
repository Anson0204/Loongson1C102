<template>
    <div id="app">
      <dv-full-screen-container>
        <router-view></router-view>
      </dv-full-screen-container>
    </div>
  </template>
  
  <script setup>
  import { onMounted } from 'vue';
  
  // Any additional setup script if needed
  onMounted(() => {
    // Any setup if needed
  });
  </script>
  
  <style>
  #app {
    font-family: Avenir, Helvetica, Arial, sans-serif;
    -webkit-font-smoothing: antialiased;
    -moz-osx-font-smoothing: grayscale;
    text-align: center;
    color: #2c3e50;
    margin-top: 60px;
  }
  
  nav {
    display: flex;
    justify-content: center;
    margin-bottom: 20px;
  }
  
  nav a {
    margin: 0 15px;
    text-decoration: none;
    color: #42b983;
  }
  </style>
