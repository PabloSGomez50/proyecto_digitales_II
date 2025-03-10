import React, { createContext, useContext, useState, useEffect } from 'react';
import { useNavigate } from 'react-router';
import { axiosAuth } from '../axiosInstance';

const UserContext = createContext();

export const useUser = () => useContext(UserContext);

export const UserProvider = ({ children }) => {
  const [user, setUser] = useState({}); // user can be null, or user data
  const navigate = useNavigate();

  const retrieveUser = () => {
    // Check for stored user info (e.g., JWT token)
    console.log("Retrieve");
    const storedUser = localStorage.getItem('user');
    console.log(JSON.parse(storedUser));
    if (storedUser) {
      setUser(JSON.parse(storedUser));
    }
    return true;
  }

  const makeDummyUser = (userData) => {
    if (userData.email_or_username.includes("@")) {
      userData.email = userData.email_or_username;
      userData.username = userData.email_or_username.split("@")[0];
    } else {
      userData.email = userData.email_or_username + "@gmail.com";
      userData.username = userData.email_or_username;
    }
    const dummy = {
      email: userData.email,
      username: userData.username
    };
    setUser(dummy);
    localStorage.setItem('user', JSON.stringify(dummy));
    navigate('/');
  }
  const register = async (userData) => {
    if (import.meta.env.VITE_DEP_ENV === 'development') {
      makeDummyUser(userData);
      return ;
    }
    try {
      const response = await axiosAuth.post("/register", userData);
      console.log(response.data);
      if (response.status == 201) {
        setUser(response.data);
        localStorage.setItem('user', JSON.stringify(response.data));
        navigate('/');
      }
    } catch (err) {
      console.error("Error in login request: ", err);
      throw err;
    }
  }

  const login = async(userData) => {
    if (import.meta.env.VITE_DEP_ENV === 'development') {
      makeDummyUser(userData);
      return ;
    }
    try {
      const response = await axiosAuth.post("/login", userData);
      console.log(response.data);
      if (response.status == 200) {
        setUser(response.data);
        localStorage.setItem('user', JSON.stringify(response.data));
        navigate('/');
      } else {
        throw new Error(response.data);
      }
    } catch (err) {
      console.error("Error in login request: ", err);
      throw err;
    }
  };

  const logout = () => {
    setUser(null);
    localStorage.removeItem('user'); // Remove stored user data
    navigate('/login');
  };

  return (
    <UserContext.Provider value={{ user, login, logout, register, retrieveUser }}>
      {children}
    </UserContext.Provider>
  );
};
