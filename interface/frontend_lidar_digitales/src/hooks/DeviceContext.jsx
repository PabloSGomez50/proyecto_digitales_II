import React, { createContext, useContext, useEffect, useState } from 'react';
import axios from 'axios';


const IP_REGEX = /\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}/;
const DeviceContext = createContext();

const default_points = {
  "distance": [20, 25, 45, 80, 80, 80, 40],
  "mot_angle": [0, 0, 450, 900, 1350, 1800, 2250],
}

export const useDevice = () => useContext(DeviceContext);

export const DeviceProvider = ({ children }) => {
  const [device, setDevice] = useState({}); // user can be null, or user data
  const [devices, setDevices] = useState([
    {id: 1, name: 'Lidar TDII AP', dns_url: 'lidar.local', 
      ip: undefined, data: {active: false, point_count: 0}, 
      points: {}
    },
  ]);
  // const navigate = useNavigate();

  const getDeviceIP = (id) => {
    const requestIP = async () => {
      try {
        const device_selected = devices.find(device => device.id === id);
        const dns_url = device_selected.dns_url;
        if (device_selected.ip) {
          return;
        }
        console.log("DNS dir to send: ", dns_url);
        const response = await axios.get(`http://${dns_url}/`);
        // Response example: Esp01 Server activo en ip: 192.168.10.161, Necesitaas
        if (!response.data) {
          return ;
        }
        const ip = response.data.match(IP_REGEX)[0];
        console.log(response.data, ip);
        if (!ip)
          return;
        setDevices(prev => {
          return prev.map(dev => {
            if (dev.id === id) {
              dev.ip = ip;
              dev.data = {...dev.data, active: true};
            }
            return dev;
          });
        });
      } catch (error) {
        console.log("getDeviceIP: ", error);
      }
    }
    requestIP();
  }

  useEffect(() => {
    devices.forEach(device => {
      if (!device.ip) {
        getDeviceIP(device.id);
      }
    });
  }, [])

  const addDevice = (device) => {
    device.id = devices.reduce((acc, dev) => (dev.id > acc) ? dev.id : acc, 0) + 1;
    console.log("addDevice: ", device); 
    if (device.ip) {
      setDevices(prev => [...prev, device]);
      return;
    }
    setDevices(prev => [...prev, {...device, ip: getDeviceIP(device.dns_url)}]);
  }

  const selectDevice = (id) => {
    const device = devices.find(device => device.id === id);
    setDevice(device);
  }

  const updateDeviceData = (id, newData) => {
    setDevices(prev => {
      return prev.map(dev => {
        if (dev.id === id) {
          dev.data = newData;
        }
        return dev;
      });
    });
    if (device.id === id) {
      setDevice(prev => {
        return {...prev, data: newData};
      });
    }
  }

  const getDeviceUrl = () => {
    if (device.ip)
      return `http://${device.ip}`;
    return `http://${device.dns_url}`;
  }

  return (
    <DeviceContext.Provider value={{ device, devices, addDevice, selectDevice, updateDeviceData, getDeviceUrl, getDeviceIP }}>
      {children}
    </DeviceContext.Provider>
  );
};
