import { useState } from "react";
import "./DeviceCommands.css";
import axios from "axios";
import { useDevice } from "../../hooks/DeviceContext";


const DeviceCommands = ({row, column}) => {
    const [stepsPerRead, setStepsPerRead] = useState(0);
    const [motorMode, setMotorMode] = useState("STEPPER_MOTOR");

    const { device, updateDeviceData } = useDevice();
    const [ inRequest, setInRequest ] = useState(false);
    const DEFAULT_VALUE = false;

    const url_create = () => {
        if (device.ip)
            return `http://${device.ip}`;
        else
            return `http://${device.dns_url}`;

    }

    const handleSendStatus = async () => {
        setInRequest(DEFAULT_VALUE);
        console.log("Sending status");
        console.log("Device: ", device.ip, device.dns_url);
        try {
            const url = url_create();
            console.log("URL to send status: ", url);
            const response = await axios.post(`${url}/msg`, {
                "message": `STATUS\n`
            });
            console.log("Response from send status: ", response.data);
            // Format: "Status alive, steps: %d, mode: %d\r\n"
        } catch (err) {
            console.error(err);
            setInRequest(false);
            return ;
        }
        setTimeout(async () => {
            try {
            let url;
            if (device.ip)
                url = `http://${device.ip}/buffer`;
            else
                url = `http://${device.dns_url}/buffer`;
            const response = await axios.get(url);
            console.log("Response from second request: ", response.data);
            const steps = response.data.buffer.match(/steps: (\d+)/);
            const mode = response.data.buffer.match(/mode: (\d+)/);
            if (!steps && !mode) {
                return;
            }
            updateDeviceData(device.id, {
                ...device.data,
                steps_per_read: steps ? parseInt(steps[1]) : undefined,
                mode: mode ? (parseInt(mode[1]) ? "Motor paso a paso" : "Motor de continua" ) : undefined
            }
            );
            setInRequest(false);
            } catch (err) {
            console.error(err);
            setInRequest(false);
            }
        }, 500);
    }

    const handlesendBattery = async () => {
        setInRequest(DEFAULT_VALUE);
        console.log("Sending Battery");
        console.log("Device: ", device.ip, device.dns_url);
        try {
            const url = url_create();
            console.log("URL to send status: ", url);
            const response = await axios.post(`${url}/msg`, {
                "message": `BATTERY\n`
            });
            console.log("Response from query battery: ", response.data);
        } catch (err) {
            console.error(err);
            setInRequest(false);
            return ;
        }
        setTimeout(async () => {
            try {
            let url;
            if (device.ip)
                url = `http://${device.ip}/buffer`;
            else
                url = `http://${device.dns_url}/buffer`;
            const response = await axios.get(url);
            console.log("Response from second request: ", response.data);
            // "Battery level: %u,%u\r\n"
            const battery = response.data.buffer.match(/Battery level: (\d+),(\d+)/);
            if (battery) {
                const battery_voltaje = Math.round((parseFloat(`${battery[1]}.${battery[2]}`) - 7.4) * 100);
                console.log("Voltaje:", battery_voltaje);
                updateDeviceData(device.id, {
                    ...device.data,
                    battery: battery_voltaje
                });
            } 
            setInRequest(false);
            } catch (err) {
            console.error(err);
            setInRequest(false);
            }
        }, 500);
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

    const handleSendSteps = async () => {
        setInRequest(DEFAULT_VALUE);
        console.log("Device: ", device.ip, device.dns_url);
        if (stepsPerRead === 0) {
            return;
        }
        try {
            const url = url_create();
            console.log("URL to send Steps: ", url);
            const response = await axios.post(`${url}/msg`, {
                "message": `M${stepsPerRead}.\n`
            });
            console.log("Response from send steps: ", response.data);
        } catch (err) {
            console.error(err);
            setInRequest(false);
            return ;
        }
        setTimeout(async () => {
            try {
            let url;
            if (device.ip)
                url = `http://${device.ip}/buffer`;
            else
                url = `http://${device.dns_url}/buffer`;
            const response = await axios.get(url);
            console.log("Response from second request: ", response.data);
            // example "Resize steps per read: %i\r\n"
            const steps = response.data.buffer.match(/Resize steps per read: (\d+)/);
            if (steps) {
                updateDeviceData(device.id, {
                    ...device.data,
                    steps_per_read: parseInt(steps[1])
                });
            }
            setInRequest(false);
            } catch (err) {
            console.error(err);
            setInRequest(false);
            }
        }, 500);
    }

    const handleSendMode = async () => {
        setInRequest(DEFAULT_VALUE);
        try {
            const url = url_create();
            console.log("URL to send Mode: ", url);
            const response = await axios.post(`${url}/msg`, {
                "message": `${motorMode}\n`
            });
            console.log("Response from send mode: ", response.data);
        } catch (err) {
            console.error(err);
            setInRequest(false);
            return ;
        }
        setTimeout(async () => {
            try {
            let url;
            if (device.ip)
                url = `http://${device.ip}/buffer`;
            else
                url = `http://${device.dns_url}/buffer`;
            const response = await axios.get(url);
            console.log("Response from second request: ", response.data);
            // "Stepper mode: %i\r\n"
            const mode = response.data.buffer.match(/Stepper mode: (\d+)/);
            if (mode) {
                if (mode[1] === "0") {
                    updateDeviceData(device.id, {
                        ...device.data,
                        mode: "Motor de continua"
                    });
                } else {
                    updateDeviceData(device.id, {
                        ...device.data,
                        mode: "Motor paso a paso"
                    });
                }
            }
            setInRequest(false);
            } catch (err) {
            console.error(err);
            setInRequest(false);
            }
        }, 500);
    }

    const handleSendReset = async () => {
        setInRequest(DEFAULT_VALUE);
        try {
            const url = url_create();
            console.log("URL to send Reset: ", url);
            const response = await axios.post(`${url}/msg`, {
                "message": `RESET\n`
            });
            console.log("Response from send reset: ", response.data);
        } catch (err) {
            console.error(err);
            setInRequest(false);
            return ;
        }
        setTimeout(async () => {
            try {
            let url;
            if (device.ip)
                url = `http://${device.ip}/buffer`;
            else
                url = `http://${device.dns_url}/buffer`;
            const response = await axios.get(url);
            console.log("Response from second request: ", response.data);
            // Example "Reset motor angle\r\n"
            const reset = response.data.buffer.match(/Reset motor angle/);
            if (reset) {
                updateDeviceData(device.id, {
                    ...device.data,
                    angle: 0
                });
            }
            setInRequest(false);
            } catch (err) {
            console.error(err);
            setInRequest(false);
            }
        }, 500);
    }

    const handleSendAngle = async () => {
        setInRequest(DEFAULT_VALUE);
        try {
            const url = url_create();
            console.log("URL to send Angle: ", url);
            const response = await axios.post(`${url}/msg`, {
                "message": `ANGLE\n`
            });
            console.log("Response from send reset: ", response.data);
        } catch (err) {
            console.error(err);
            setInRequest(false);
            return ;
        }
        setTimeout(async () => {
            try {
            let url;
            if (device.ip)
                url = `http://${device.ip}/buffer`;
            else
                url = `http://${device.dns_url}/buffer`;
            const response = await axios.get(url);
            console.log("Response from second request: ", response.data);
            // Example: "Current angle: %i\r\n"
            const angle = response.data.buffer.match(/Current angle: (\d+)/);
            if (angle) {
                updateDeviceData(device.id, {
                    ...device.data,
                    angle: Math.round(parseInt(angle[1]) / 10)
                });
            }
            setInRequest(false);
            } catch (err) {
            console.error(err);
            setInRequest(false);
            }
        }, 500);
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
                {device?.name ? (
                <>
                    <div className="device-command-row">
                        <label>Actualizar información:</label>
                        <div/>
                        <button className="btn" onClick={handleSendStatus}
                            disabled={inRequest}
                        >Actualizar</button>
                        <div></div>
                    </div>
                    <div className="device-command-row">
                        <label>Nivel de bateria:</label>
                        <div/>
                        <button className="btn" onClick={handlesendBattery}
                            disabled={inRequest}
                        >Actualizar</button>
                        <span>{device?.data?.battery ? device?.data?.battery + "%": "No disponible"}</span>
                    </div>
                    <div className="device-command-row">
                        <label>Angulo Actual:</label>
                        <div></div>
                        {/* <button
                        className="btn"
                        onClick={handleSendReset}
                        disabled={inRequest}
                        >
                            Reiniciar
                        </button> */}
                        <button
                        className="btn"
                        onClick={handleSendAngle}
                        disabled={inRequest}
                        >
                            Actualizar
                        </button>
                        <span>{device?.data?.angle !== undefined ? (device.data.angle % 360) + "°": "No disponible"}</span>
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
                        <button
                            className="btn"
                            onClick={handleSendSteps}
                            disabled={inRequest}
                        >Enviar</button>
                        <div></div>
                    </div>
                    <div className="device-command-row">
                        <label htmlFor="motorMode">Cambiar modo de motor:</label>
                        <select
                            id="motorMode"
                            value={motorMode}
                            onChange={(e) => setMotorMode(e.target.value)}
                        >
                            <option value="STEPPER_MOTOR">Modo Stepper</option>
                            <option value="DC_MOTOR">Modo DC</option>
                        </select>
                        <button
                            className="btn"
                            onClick={handleSendMode}
                            disabled={inRequest}
                        >Enviar</button>
                        <div></div>
                    </div>
                </>
                )
                :
                <div style={{
                    display: "flex",
                    justifyContent: "center",
                    alignItems: "center",
                    height: "100%"
                }}>
                    <p>Se debe seleccionar un dispositivo</p>
                </div>
                }
                
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