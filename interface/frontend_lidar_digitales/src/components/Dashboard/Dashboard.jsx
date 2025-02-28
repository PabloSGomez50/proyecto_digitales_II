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
    const { device } = useDevice();

    const handleCheckDevice = async () => {
        try {
            let url;
            if (device.ip)
                url = `http://${device.ip}/`;
            else
                url = `http://${device.dns_url}/`;
            const response = await axios.get(url);
            console.log("Response from check device: ", response.data);
        } catch (err) {
            console.log(err);
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
                <div className="dashboard-status">
                    {device?.name ?
                    <>
                        <span>Estado de {device.name}: </span>
                        {device?.active ? 
                        <span className='dev-active'>Activo</span>
                        :
                        <span className='dev-inactive'>Inactivo</span>
                        }
                        
                        <IoReloadCircle className="small-icon" onClick={handleCheckDevice}/>
                    </>
                    :
                    <span>Seleccionar un dispositivo en el Header</span>
                    }
                </div>
                
            </div>
            <div
                className="dashboard-data"
                style={{
                    gridTemplateColumns: "repeat(5, 1fr)",
                    gridTemplateRows: "repeat(4, 1fr)"
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
                    data={{name: 'Puntos Guardados', value: device?.points?.distance?.length, default: 0}}
                    row={2}
                    column={1}
                />
                <DataChip
                    Icon={FaPlus}
                    data={{name: 'Vueltas completadas', value: Math.trunc(device?.data?.angle / 360), default: 0}}
                    row={2}
                    column={2}
                />

                <DataProgression
                    data={device?.points}
                    row={"1 / 5"}
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
                    row={"3 / 5"}
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