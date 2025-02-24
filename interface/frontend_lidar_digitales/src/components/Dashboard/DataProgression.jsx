import "./DataProgression.css";


const DataProgression = ({ data, row, column }) => {
    return (
        <div className='data-progression' style={{gridRow: row, gridColumn: column}}>
            <div className='data-progression-header'>
                <p>Progresión</p>
            </div>
            <div className='data-progression-body'>
                <p>
                    Graph
                </p>
            </div>
        </div>
    )
}

export default DataProgression;