import "./DataProgression.css";
import React, { useEffect, useRef, useState } from 'react';
import Plotly from 'plotly.js-dist-min'
import { useDevice } from "../../hooks/DeviceContext";
import axios from "axios";



const DataProgression = ({ data, row, column }) => {
    const graphRef = useRef();
    const [status, setStatus] = useState();
    const [points, setPoints] = useState();
    const interval_ref = useRef();
    // const data = {
    //     "distance": [20, 25, 45, 80, 80, 80, 40],
    //     "mot_angle": [0, 0, 450, 900, 1350, 1800, 2250],
    //   }

    const { device, getDeviceUrl } = useDevice();

    useEffect(() => {
        setPoints(device?.points || {"distance": [], "mot_angle": []});
    }, [device?.id]);

    const handleSendStart = async () => {
        setStatus("Enviando...");
        let state = false;
        try {
            const url = getDeviceUrl();
            console.log("URL to send Start: ", url);
            const response = await axios.post(`${url}/active`);
            console.log("Response from send Start: ", response.data);
            if(response.data?.msg?.match("ACTIVE")) {
                state = true;
                console.log("State: ", state);
            }
        } catch (err) {
            console.error(err);
            setStatus("Error");
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
            // Example: "New menu selected: ACTIVE\r\n"
            if (response.data?.buffer?.match("ACTIVE") || state) {
                setStatus("Activo");
            }
            } catch (err) {
            console.error(err);
            setStatus("Error");
            }
        }, 100);
    }
    const handleSendStop = async () => {
        setStatus("Enviando...");
        try {
            const url = getDeviceUrl();
            console.log("URL to send Stop: ", url);
            const response = await axios.post(`${url}/idle`);
            console.log("Response from send reset: ", response.data);
        } catch (err) {
            console.error(err);
            setStatus("Error");
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
            // Example: "New menu selected: IDLE\r\n"
            if (response.data?.buffer.match("IDLE")) {
                setStatus("Detenido");
            }
            } catch (err) {
                console.error(err);
                setStatus("Error");
            }
        }, 100);
    }

    const requestData = async () => {
        try {
            const url = `${getDeviceUrl()}/sensor`;
            console.log("URL to request sensor data: ", url);
            const response = await axios.get(url);
            // console.log("Response: ", response.data);
            if (response.data?.distance && response.data?.mot_angle) {
                setPoints(prev => ({
                    "distance": prev?.distance ? prev.distance.concat(response.data.distance) : response.data.distance,
                    "mot_angle": prev?.mot_angle ? prev.mot_angle.concat(response.data.mot_angle) : response.data.mot_angle
                }));
            }

        } catch (err) {
            console.error(err);
            setStatus("Error");
            return ;
        }
    }

    useEffect(() => {
        // if (status === "Enviando...") {
        //     setTimeout(() => {
        //         setStatus(undefined);
        //     }, 1000);
        // }
        if (status === "Activo") {
            interval_ref.current = setInterval(requestData, 1000);
        }
        if (status === "Detenido") {
            clearInterval(interval_ref.current);
        }

        return () => {
            if (interval_ref.current) {
                clearInterval(interval_ref.current);
            }
        }
    }, [status]);

    useEffect(() => {
        const handleResize = () => {
            Plotly.Plots.resize(graphRef.current);
        };

        window.addEventListener('resize', handleResize);

        return () => {
            window.removeEventListener('resize', handleResize);
        };
    }, []);
    
    useEffect(() => {
        console.log("Update chart with points: ", points);
        const mot_angle = points?.mot_angle ? points.mot_angle : [];
        const distance = points?.distance ? points.distance : [];
        const trace = {
            r: distance,
            theta: mot_angle.map(item => item / 10),
            mode: 'markers',
            type: 'scatterpolar'
        };

        const layout = {
            polar: {
                radialaxis: {
                    visible: true,
                    range: [0, Math.max(...distance) + 20]
                }
            },
            showlegend: false
        };

        Plotly.newPlot(graphRef.current, [trace], layout);
    }, [points]);

    return (
        <div className='data-progression flex-col' style={{gridRow: row, gridColumn: column}}>
            <div className='data-progression-header'>
                <p>Grafico Detección de Obstáculos</p>
            </div>
            <div className='data-progression-body'>
                <div ref={graphRef}></div>
                <div className="data-progression-options flex gap-1">
                    <button className="btn" onClick={() => setPoints({"distance": [], "mot_angle": []})}>
                        <span>Eliminar contenido</span>
                        {/* <i className="fas fa-download"></i> */}
                    </button>
                    <button className="btn" onClick={handleSendStart}>
                        <span>Iniciar</span>
                    </button>
                    <button className="btn" onClick={handleSendStop}>
                        <span>Detener</span>
                    </button>
                </div>
                <div className="flex gap-1">
                    <span>Estado de dispositivo:</span>
                    <span className="msg-status">
                        {status !== undefined ?
                        status
                        :
                        "Desconocido"
                        }
                    </span>
                </div>
            </div>
        </div>
    )
}

export default DataProgression;