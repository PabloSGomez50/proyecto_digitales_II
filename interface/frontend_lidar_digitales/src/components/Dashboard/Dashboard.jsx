import { useEffect, useState } from "react";
import { useUser } from "../../hooks/UserContext";
import './Dashboard.css';
import WilliamsLogo from '../../assets/imgs/Logo_Williams_F1.png';
import axios from 'axios';

import { FaPlus } from "react-icons/fa6";
import { IoReloadCircle } from "react-icons/io5";
import DataChip from "./DataChip";

import DataProgression from "./DataProgression";

import { useDevice } from "../../hooks/DeviceContext";
import DeviceCommands from "../Devices/DeviceCommands";


const Dashboard = () => {
    
    const { user } = useUser();
    const { device, getDeviceUrl, getDeviceIP, updateDeviceData } = useDevice();

    const handleCheckDevice = async () => {
        console.log("Check Device: ", device);
        getDeviceIP(device.id);
        console.log("Device IP: ", device.ip);
        try {
            const url = getDeviceUrl();
            const response = await axios.get(url);
            console.log("Response from check device: ", response.data);
            //"Esp01 Server activo en ip: {ipaddress}, Necesitaas mas informacion?
            const ip = response.data?.match(/(\d{1,3}\.){3}\d{1,3}/)[0];
            console.log("IP: ", ip);

            if (ip) {
                updateDeviceData(device.id, {...device.data, active: true});
            } else {
                updateDeviceData(device.id, {...device.data, active: false});
            }
        } catch (err) {
            console.log(err);
            updateDeviceData(device.id, {...device.data, active: false});
            return ;
        }
    }


    return (
        <section className='dashboard-container'>
            <div className="dashboard-header gap-0-5">
                <div className="dashboard-profile">
                    <img src={user?.profile_img || WilliamsLogo} className='profile-icon-img' alt='Page icon'/>
                    <p>{user?.email}</p>
                </div>
                <div className="gap-0-5 flex">
                    {device?.name ?
                    <>
                        <span>Estado de {device.name}: </span>
                        {device?.data?.active ? 
                        <span className='dev-active'>Activo</span>
                        :
                        <span className='dev-inactive'>Inactivo</span>
                        }
                        
                        <IoReloadCircle className="small-icon pointer" onClick={handleCheckDevice}/>
                    </>
                    :
                    <span>Seleccionar un dispositivo en el Header</span>
                    }
                </div>
                
            </div>
            <div
                className="dashboard-data"
                style={{
                    gridTemplateColumns: "repeat(4, 1fr)",
                    gridTemplateRows: "repeat(5, 8rem)"
                }}
            >
                <DataChip
                    Icon={FaPlus}
                    data={{name: 'Modo motor', value: device?.data?.mode, default: "Desconocido"}}
                    row={1}
                    column={1}
                />
                <DataChip
                    Icon={FaPlus}
                    data={{name: 'Pasos por lectura', value: device?.data?.steps_per_read}}
                    row={1}
                    column={2}
                />
                <DataChip 
                    Icon={FaPlus}
                    data={{name: 'Puntos Guardados', value: device?.data?.point_count, default: 0}}
                    row={2}
                    column={1}
                />
                <DataChip
                    Icon={FaPlus}
                    data={{name: 'Vueltas completadas', value: device?.data?.mot_lap, default: 0}}
                    row={2}
                    column={2}
                />

                <DataProgression
                    // data={device?.points}
                    row={"1 / 6"}
                    column={"3 / 7"}
                />
                {/* <DataChip
                    Icon={FaPlus}
                    data={{name: 'Data', value: device?.data?.current_lap, default: 0}}
                    row={4}
                    column={3}
                /> */}

                {/* <RankTable
                    data={device?.data?.ranking || []}
                    row={"3 / 5"}
                    column={"1 / 3"}
                /> */}
                <DeviceCommands
                    data={device?.data?.ranking || []}
                    row={"3 / 6"}
                    column={"1 / 3"}
                />
                {/*
                <MediumGraph
                    title="Proximos partidos"
                    data={device?.data?.duels || []}
                    row={"3 / 5"}
                    column={"3 / 5"}
                />
                <MediumGraph
                    title="Calendario desafios"
                    data={device?.data?.calendar || []}
                    row={"3 / 5"}
                    column={"5 / 7"}
                /> */}
            </div>

        </section>
    )
}

export default Dashboard;