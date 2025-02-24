

const DataChip = ({ data, row, column, Icon }) => {
    return (
        <div className='data-chip' style={{gridRow: row, gridColumn: column}}>
            <Icon className="icon" />
            <div>
                <p>{data.name}</p>
                <p>{data.value || 0}</p>
            </div>
        </div>
    )
}

export default DataChip;