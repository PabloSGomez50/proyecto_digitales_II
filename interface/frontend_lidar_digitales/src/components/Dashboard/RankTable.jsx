import "./RankTable.css";


const RankTable = ({ data, row, column }) => {
    return (
        <div className='rank-table' style={{gridRow: row, gridColumn: column}}>
            <div className='rank-table-header'>
                <p>Ranking</p>
                <button
                
                >
                    + Agregar usuario
                </button>
            </div>
            <div className='rank-table-body'>
                <div className='rank-table-row'>
                    <span>NOMBRE</span>
                    <span>POSICIÓN</span>
                    <span>PUNTOS</span>
                </div>
                {data.map((user, index) => (
                    <div key={user.id} className='rank-table-row'>
                        <span>{user.name}</span>
                        <span>{index + 1}</span>
                        <span>{user.points}</span>
                    </div>
                ))}
            </div>
        </div>
    )
}

export default RankTable;