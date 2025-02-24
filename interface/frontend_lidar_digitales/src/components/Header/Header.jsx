import './Header.css';
import { FaPlus} from "react-icons/fa6";
import { FaSearch } from "react-icons/fa";
// import Modal from '../Modal/Modal';
import { useEffect, useState } from 'react';
// import FormContainer from './FormContainer';
import { useUser } from '../../hooks/UserContext';
import { axiosContent } from '../../axiosInstance';


const Header = ({  }) => {
    const { user } = useUser();
    const [showModal, setShowModal] = useState(false);
    const [showDuels, setShowDuels] = useState(false);
    const [data, setData] = useState([]);

    const handleDuelSelected = (duelSelected) => {
        setShowDuels(false);
        setDuelSelected(duelSelected);
    }
    
    const sendData = (e) => {
        e.preventDefault();
        const dataToSend = {};
        data.forEach(item => {
            dataToSend[item.id] = item.value;
        });
        dataToSend.owner_id = user.id;
        console.log("Data to send: ", dataToSend);
        const createMatchRq = async (data) => {
            try {
                const response = await axiosContent.post('/matches/', data);
                console.log(response.data);
            } catch (error) {
                console.log(error);
            }
        }
        // createMatchRq(dataToSend);
        setShowModal(false);
    }

    // useEffect(() => {
    //     setData(prev => {
    //         return prev.map(item => {
    //             if (item.id === 'game_id') {
    //                 return {
    //                     ...item,
    //                     options: games
    //                 }
    //             }
    //             return item;
    //         })
    //     });
    // }, [games])

    return (
    <header className='noselect'>
        <div
            className={`header-title ${duelSelected ? 'selected' : ''}`}
            onMouseEnter={() => setShowDuels(true)}
            onMouseLeave={() => setShowDuels(false)}
        >
            {showDuels ?
                <span>Seleccionar duelo</span>
                :
                <span>{duelSelected}</span>
            }
            
          <ul className={`header-duels ${showDuels ? 'show' : ''}`}>
            {duels.map(item =>
              <li
                onClick={() => handleDuelSelected(item)}
                key={item}
              >
                {item}
              </li>
            )}
          </ul> 
        </div>
        <div className='header-content'>
            <div className='searchbar flex'>
                <FaSearch  className='icon'/>
                <input type="text" name="search" />
            </div>
            <button 
                className='header-new-duel flex'
                onClick={() => setShowModal(true)}
            >
                <FaPlus className='icon' />
                <span>Nuevo duelo</span>
            </button>
            {showModal && 
                <Modal onClose={() => setShowModal(false)}>
                    <FormContainer
                        data={data}
                        setData={setData}
                        sendData={sendData}
                    />
                </Modal>
            }
        </div>
    </header>
    )
}

export default Header;