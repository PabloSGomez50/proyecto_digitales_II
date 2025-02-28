import axios from 'axios';

// Base URL and API key from environment variables
const BASE_URL = import.meta.env.VITE_API_URL;
const API_KEY = import.meta.env.VITE_API_KEY;

// Axios instance with headers (includes API key)
export const axiosAuth = axios.create({
  baseURL: BASE_URL + '/users',
  headers: {
    'Content-Type': 'application/json',
    'Authorization': `Bearer ${API_KEY}`,
  },
});

// Axios instance without headers

export const axiosContent = axios.create({
  baseURL: BASE_URL,
  headers: {
    'Content-Type': 'application/json',
    'Authorization': `Bearer ${API_KEY}`,
  },
});

export const axiosRq = axios.create({
  baseURL: BASE_URL,
});