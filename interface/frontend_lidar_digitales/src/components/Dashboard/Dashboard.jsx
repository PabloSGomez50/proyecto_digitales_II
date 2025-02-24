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

const api_url = "";

const Dashboard = () => {
    
    const {user} = useUser();
    const [data, setData] = useState(undefined);

    useEffect(() => {
        const fetchData = async (user_id) => {
            try {
                const response = await axios.get(`${api_url}/user/${user_id}`, {
                    headers: {
                        Authorization: `Bearer ${user.token}`
                    }
                });
                setData(response.data.values);
            } catch (error) {
                console.error('Error fetching data:', error);
            }
        }
        // fetchData();
        setData({
            ranking: [
                {id: 1, name: 'Pablo', points: 100},
                {id: 2, name: 'Martu', points: 90},
            ]
        });
    }, []);
    return (
        <section className='dashboard-container'>
            <div className="dashboard-header">
                <div className="dashboard-profile">
                    <img src={user?.profile_img || WilliamsLogo} className='profile-icon-img' alt='Page icon'/>
                    <p>{user?.email}</p>
                </div>
                <div>
                    <button>Resumen</button>
                    <button>Amigos</button>
                    <button>Historial</button>
                </div>
            </div>
            <div className="dashboard-data">
                <DataChip
                    Icon={FaPlus}
                    data={{name: 'Puntos', value: data?.points}}
                    row={1}
                    column={1}
                />
                <DataChip
                    Icon={FaPlus}
                    data={{name: 'Juegos Realizados', value: data?.points}}
                    row={1}
                    column={2}
                />
                <DataChip
                    Icon={FaPlus}
                    data={{name: 'Duelos Ganados', value: data?.points}}
                    row={2}
                    column={1}
                />
                <DataChip
                    Icon={FaPlus}
                    data={{name: 'Duelos Perdidos', value: data?.points}}
                    row={2}
                    column={2}
                />

                <RankTable
                    data={data?.ranking || []}
                    row={"3 / 5"}
                    column={"1 / 3"}
                />

                <DataProgression
                    data={data?.progression || []}
                    row={"1 / 3"}
                    column={"3 / 7"}
                />

                <MediumGraph
                    title="Proximos partidos"
                    data={data?.duels || []}
                    row={"3 / 5"}
                    column={"3 / 5"}
                />
                <MediumGraph
                    title="Calendario desafios"
                    data={data?.calendar || []}
                    row={"3 / 5"}
                    column={"5 / 7"}
                />
            </div>

        </section>
    )
}

export default Dashboard;