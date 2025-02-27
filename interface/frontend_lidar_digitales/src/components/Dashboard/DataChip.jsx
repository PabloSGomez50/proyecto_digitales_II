

const DataChip = ({ data, row, column, Icon }) => {
    return (
        <div className='data-chip' style={{gridRow: row, gridColumn: column}}>
            <div className="flex gap-1">
                <Icon className="small-icon" />
                <p>{data.name}</p>
            </div>
            <p>{data?.value || data?.default || 0}</p>
        </div>
    )
}

export default DataChip;