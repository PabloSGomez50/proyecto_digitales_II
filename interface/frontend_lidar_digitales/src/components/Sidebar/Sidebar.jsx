import './Sidebar.css';
import WilliamsLogo from '../../assets/imgs/Logo_Williams_F1.png';
import { FaGear, FaDatabase } from "react-icons/fa6";
import { IoIosArrowDropdown } from "react-icons/io";
import { MdLogout } from "react-icons/md";

import SidebarOption from './SidebarOption';
import { useUser } from '../../hooks/UserContext';


const Sidebar = ( {
    sidebarOptions,
    optionSelected , setOptionSelected
}) => {

  const { user, logout } = useUser();

    return (
        <aside className='sidebar noselect'>
            <div className='profile-container'>
              <img src={user?.profile_img || WilliamsLogo} className='profile-icon-img' alt='Page icon'/>

              <div className="flex">
                  <div className='profile-info'>
                      <span>{user?.username}</span>
                      <span>{user?.email}</span>
                  </div>
                  <MdLogout
                    className='small-icon cursor'
                    onClick={() => logout()}
                  />
              </div>
            </div>
        <div className='sidebar-options'>
          {sidebarOptions.map(item =>
            <SidebarOption
              key={item.name}
              Icon={item.icon}
              title={item.name}
              selected={optionSelected}
              setSelected={setOptionSelected}
            />

          )}
        </div>
        {/* <div className='sidebar-config'>
          <SidebarOption 
            Icon={FaGear}
            title='Configuración'
            selected={optionSelected}
            setSelected={setOptionSelected}
          />
        </div> */}
      </aside>
    )
}

export default Sidebar;