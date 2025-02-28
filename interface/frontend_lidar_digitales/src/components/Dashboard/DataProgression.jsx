import "./DataProgression.css";
import React, { useEffect, useRef } from 'react';
import Plotly from 'plotly.js-dist-min'



const DataProgression = ({ data, row, column }) => {
    const graphRef = useRef();
    // const data = {
    //     "distance": [20, 25, 45, 80, 80, 80, 40],
    //     "mot_angle": [0, 0, 450, 900, 1350, 1800, 2250],
    //   }
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
        console.log(data);
        const mot_angle = data?.mot_angle ? data.mot_angle : [];
        const distance = data?.distance ? data.distance : [];
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
    }, [data]);

    return (
        <div className='data-progression flex-col' style={{gridRow: row, gridColumn: column}}>
            <div className='data-progression-header'>
                <p>Grafico Detección de Obstáculos</p>
            </div>
            <div className='data-progression-body'>
                <div ref={graphRef}></div>
            </div>
        </div>
    )
}

export default DataProgression;