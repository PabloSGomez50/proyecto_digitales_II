import { useNavigate } from "react-router";

const SidebarOption = ({ Icon, title, selected, setSelected }) => {
    const navigate = useNavigate();
    const handleClick = () => {
        setSelected(title.toLowerCase());
        if (title == 'Dashboard') {
            navigate('/');
        } else {
            navigate(`/${title.toLowerCase()}`);
        }
    }

    return (
        <div 
            className={`sidebar-option ${selected == title.toLowerCase() && 'sidebar-option-active'}`}
            onClick={handleClick}
        >
            <Icon className="small-icon" />
            <h3>{title}</h3>
        </div>
    )
}

export default SidebarOption;