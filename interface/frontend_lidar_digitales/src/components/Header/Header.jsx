import './Header.css';
import { FaPlus} from "react-icons/fa6";
import { FaSearch } from "react-icons/fa";
import { useEffect, useState } from 'react';

import { useUser } from '../../hooks/UserContext';
import { useDevice } from '../../hooks/DeviceContext';

import Modal from '../Modal/Modal';
import FormContainer from '../Basic/FormContainer';


const Header = () => {
    // const { user } = useUser();
    const [currentTime, setCurrentTime] = useState(new Date().toLocaleTimeString());
    const { device, devices, selectDevice, addDevice } = useDevice();
    const [showModal, setShowModal] = useState(false);
    const [ShowDevices, setShowDevices] = useState(false);
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

    const handleDropDownSelected = (item) => {
        setShowDevices(false);
        selectDevice(item);
    }

    useEffect(() => {
        const timer = setInterval(() => {
            setCurrentTime(new Date().toLocaleTimeString('es-es', { hour12: false }));
        }, 1000);

        return () => clearInterval(timer);
    }, []);

    return (
    <header className='noselect'>
        <div
            className={`header-title ${device?.id ? 'selected' : ''}`}
            onMouseEnter={() => setShowDevices(true)}
            onMouseLeave={() => setShowDevices(false)}
        >
            {(ShowDevices || device?.name === undefined)  ?
                <span>Seleccionar dispositivo</span>
                :
                <span>{device?.name}</span>
            }
            
          <ul className={`header-duels ${ShowDevices ? 'show' : ''}`}>
            {devices.map(item =>
              <li
                key={item.id}
                onClick={() => handleDropDownSelected(item.id)}
              >
                {item?.name}
              </li>
            )}
          </ul> 
        </div>
        <div className='header-content'>
            <button 
                className='header-new-duel flex'
                onClick={() => setShowModal(true)}
            >
                <FaPlus className='icon' />
                <span>Nuevo Dispositivo</span>
            </button>
            {/* <div className='searchbar flex'>
                <FaSearch  className='icon'/>
                <input type="text" name="search" />
            </div> */}


            <div className='header-time'>
                {currentTime}
            </div>
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
        </div>
    </header>
    )
}

export default Header;