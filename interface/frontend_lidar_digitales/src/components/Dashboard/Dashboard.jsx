import { useEffect, useState } from "react";
import { useUser } from "../../hooks/UserContext";
import './Dashboard.css';
import WilliamsLogo from '../../assets/imgs/Logo_Williams_F1.png';
import axios from 'axios';

import { FaPlus } from "react-icons/fa6";
import DataChip from "./DataChip";
import RankTable from "./RankTable";
import DataProgression from "./DataProgression";
import MediumGraph from "./MediumGraph";
import { useDevice } from "../../hooks/DeviceContext";
import DeviceCommands from "../Devices/DeviceCommands";


const Dashboard = () => {
    
    const { user } = useUser();
    const { device } = useDevice();


    return (
        <section className='dashboard-container'>
            <div className="dashboard-header gap-0-5">
                <div className="dashboard-profile">
                    <img src={user?.profile_img || WilliamsLogo} className='profile-icon-img' alt='Page icon'/>
                    <p>{user?.email}</p>
                </div>
                <div>
                    <span>Estado de {device?.name}: </span>
                    {device?.active ? 
                        <span className='dev-active'>Activo</span>
                     :
                        <span className='dev-inactive'>Inactivo</span>
                    }
                </div>
                {/* <div>
                    <button>Resumen</button>
                    <button>Amigos</button>
                    <button>Historial</button>
                </div> */}
            </div>
            <div className="dashboard-data">
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
                    data={{name: 'Puntos Guardados', value: device?.points?.length, default: 0}}
                    row={2}
                    column={1}
                />
                <DataChip
                    Icon={FaPlus}
                    data={{name: 'Vueltas completadas', value: device?.data?.current_lap, default: 0}}
                    row={2}
                    column={2}
                />

                <DataProgression
                    data={device?.points}
                    row={"1 / 5"}
                    column={"3 / 7"}
                />

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