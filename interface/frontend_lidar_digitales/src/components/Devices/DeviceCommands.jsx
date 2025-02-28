import { useState } from "react";
import "./DeviceCommands.css";


const DeviceCommands = ({row, column}) => {
    const [stepsPerRead, setStepsPerRead] = useState(0);
    const [batteryLevel, setBatteryLevel] = useState(0);
    const [motorMode, setMotorMode] = useState(0);
    const [angleValue, setAngleValue] = useState();

    const handleMotorMode = (e) => {
        setMotorMode(e.target.value);
    }

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
            </div>
            <div className='device-command-body'>
                <div className='device-command-row'>
                    <span>NOMBRE</span>
                    <span>INTERACCIONES</span>
                    <span></span>
                    <span>ESTADO</span>
                </div>
                <div className="device-command-row">
                    <label>Actaulizar información:</label>
                    <div/>
                    <button className="btn">Actualizar</button>
                    <div></div>
                </div>
                <div className="device-command-row">
                    <label>Nivel de bateria:</label>
                    <div/>
                    <button className="btn">Actualizar</button>
                    <span>{batteryLevel ? batteryLevel + "%": "No disponible"}</span>
                </div>
                <div className="device-command-row">
                    <label>Angulo Actual:</label>
                    <button className="btn">Reiniciar</button>
                    <button className="btn">Actualizar</button>
                    <span>{angleValue !== undefined ? angleValue + "°": "No disponible"}</span>
                </div>
                <div className="device-command-row">
                    <label htmlFor="stepPerRead">Pasos por lectura:</label>
                    <input
                        id="stepPerRead"
                        value={stepsPerRead}
                        onChange={handleStepsPerRead}
                        type="text"
                        placeholder="Pasos por lectura"
                    />
                    <button className="btn">Enviar</button>
                    <div></div>
                </div>
                <div className="device-command-row">
                    <label htmlFor="motorMode">Cambiar modo de motor:</label>
                    <select
                        id="motorMode"
                        value={motorMode}
                        onChange={handleMotorMode}
                    >
                        <option value="0">Modo Stepper</option>
                        <option value="1">Modo DC</option>
                    </select>
                    <button className="btn">Enviar</button>
                    <div></div>
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