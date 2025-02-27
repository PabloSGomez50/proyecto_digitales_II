import { useState } from "react";
import "./DeviceCommands.css";


const DeviceCommands = ({row, column}) => {
    const [stepsPerRead, setStepsPerRead] = useState(0);

    const handleStepsPerRead = (e) => {
        const value = e.target.value;
        if (/^\d*$/.test(value)) {
            const valueInt = parseInt(value);
            if(valueInt < 100 && valueInt > 0) {
                setStepsPerRead(valueInt);
            }
        }
        if (value === "") {
            setStepsPerRead(0);
        }
    }
    return (
        <div className='device-command' style={{gridRow: row, gridColumn: column}}>
            <div className='device-command-header'>
                <p>Comandos disponibles</p>
                {/* <button
                
                >
                    + Agregar usuario
                </button> */}
            </div>
            <div className='device-command-body'>
                <div className='device-command-row'>
                    <span>NOMBRE</span>
                    <span></span>
                    <span>ESTADO</span>
                </div>
                <div className="device-command-row">
                    <input
                        value={stepsPerRead}
                        onChange={handleStepsPerRead}
                        type="text"
                        placeholder="Pasos por lectura"
                    />
                    <button className="btn">Enviar</button>
                </div>
                <div className="device-command-row">
                    <input
                        value={stepsPerRead}
                        onChange={handleStepsPerRead}
                        type="text"
                        placeholder="Pasos por lectura"
                    />
                </div>
                {/* 
                {data.map((user, index) => (
                    <div key={user.id} className='device-command-row'>
                        <span>{user.name}</span>
                        <span>{index + 1}</span>
                        <span>{user.points}</span>
                    </div>
                ))} 
                */}
            </div>
        </div>
    )
}

export default DeviceCommands;