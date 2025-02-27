import React, { useState } from 'react';
import './Devices.css';
import FormContainer from '../Basic/FormContainer';
import Modal from '../Modal/Modal';
import { useDevice } from '../../hooks/DeviceContext';

const Devices = () => {
    const { devices, addDevice } = useDevice();
  const [showModal, setShowModal] = useState(false);
  const [data, setData] = useState([
    {
        id: "name",
        name: "Nombre del dispositivo",
        type: "text",
        value: "",
        required: true
    },
    {
        id: "dns_url",
        name: "Dirección DNS",
        type: "text",
        value: "",
        required: true
    },
    {
        id: "ip",
        name: "Dirección IP",
        type: "text",
        value: ""
    }
]);

  return (
    <div className="devices-container">
      <h2>Devices</h2>
      <p>Count of devices added: {devices.length}</p>
      <button onClick={() => setShowModal(true)}>Add New Device</button>

        {showModal && 
            <Modal onClose={() => setShowModal(false)}>
                <FormContainer
                    data={data}
                    setData={setData}
                    sendData={addDevice}
                    onClose={() => setShowModal(false)}
                />
            </Modal>
        }

      <div className="devices-grid">
        {devices.map((device, index) => (
          <div key={index} className="device-card">
            <h3>{device.name}</h3>
            <p>IP: {device.ip}</p>
            <button onClick={() => alert(`IP: ${device.ip}`)}>Get IP</button>
            <button onClick={() => alert(`Status of ${device.name}`)}>Consult Status</button>
          </div>
        ))}
      </div>
    </div>
  );
};

export default Devices;