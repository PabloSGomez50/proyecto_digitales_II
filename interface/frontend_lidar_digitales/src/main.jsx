import { createRoot } from 'react-dom/client';
import { BrowserRouter, Routes, Route } from "react-router";
import { UserProvider } from './hooks/UserContext';
import './index.css'
import App from './App.jsx'
import Login from './components/Auth/Login.jsx';
import Register from './components/Auth/Register.jsx';
import AuthReq from './components/Auth/AuthReq.jsx';
import AuthLayout from './components/Auth/AuthLayout.jsx';
import { DeviceProvider } from './hooks/DeviceContext.jsx';
// import About from './components/About.jsx';


createRoot(document.getElementById('root')).render(
  <BrowserRouter>
  <DeviceProvider>
  <UserProvider>
    <Routes>
      <Route element={<AuthLayout />}>
        <Route path="login" element={<Login />} />
        <Route path="register" element={<Register />} />
      </Route>

      <Route element={<AuthReq />}>
        <Route index element={<App />} path="/*" />
        {/* <Route path="about" element={<About />} /> */}
      </Route>
    </Routes>
  </UserProvider>
  </DeviceProvider>
  </BrowserRouter>
);
