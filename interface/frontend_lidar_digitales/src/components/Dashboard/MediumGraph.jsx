import "./MediumGraph.css";

const MediumGraph = ({ title, data, row, column }) => {
    return (
        <div className='medium-graph' style={{gridRow: row, gridColumn: column}}>
            <div className='medium-graph-header'>
                <p>{title}</p>
            </div>
            <div className='medium-graph-body'>
                <p>
                    {data}
                </p>
            </div>
        </div>
    )
}

export default MediumGraph;